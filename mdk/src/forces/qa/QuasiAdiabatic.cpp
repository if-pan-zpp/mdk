#include "cpu/forces/qa/QuasiAdiabatic.hpp"
#include <algorithm>
using namespace mdk;
using namespace mdk::param;

QuasiAdiabatic::QuasiAdiabatic(Model const& model, const Parameters &params) {
    types = Types(model);
    seqs = Chains(model);

    for (auto const& acid: AminoAcid::aminoAcids()) {
        auto const& spec = params.specificity.at(acid);
        specs[(int8_t)acid] = spec;
    }

    bb_lj = LennardJones(5.0 * angstrom, 1.0 * eps);
    bs_lj = LennardJones(6.8 * angstrom, 1.0 * eps);

    for (auto const& [acids, r]: params.pairwiseMinDist) {
        auto acid1 = (int8_t)acids.first, acid2 = (int8_t)acids.second;
        auto sslj = SidechainLJ(1.0 * eps, r);
        ss_ljs[acid1][acid2] = ss_ljs[acid2][acid1] = sslj;
    }

    n = h = Vectors(model.n);
}

bool QuasiAdiabatic::geometryPhase(PairInfo const& p, QADiff &diff) const {
    Vector h1 = h[p.i1], h2 = h[p.i2];
    double cos_h1_r12 = h1.dot(p.unit), cos_h2_r12 = h2.dot(p.unit),
        cos_h1_h2 = h1.dot(h2);

    if (abs(cos_h1_r12) >= hr_abs_min && abs(cos_h2_r12) >= hr_abs_min &&
        abs(cos_h1_h2) >= hh_abs_min && p.norm <= bb_lj.r_min) {

        diff.cont.type = QAContact::Type::BB;
        return true;
    }

    Vector n1 = n[p.i1];
    double cos_n1_r12 = n1.dot(p.unit);
    if (cos_n1_r12 <= nr_max && abs(cos_h1_r12) >= hr_abs_min &&
        p.norm <= bs_lj.r_min) {

        diff.cont.type = QAContact::Type::BS;
        return true;
    }

    Vector n2 = n[p.i2];
    double cos_n2_r12 = n2.dot(p.unit);
    if (cos_n2_r12 <= nr_max && abs(cos_h1_r12) >= hr_abs_min &&
        p.norm <= bs_lj.r_min) {

        diff.cont.type = QAContact::Type::SB;
        return true;
    }

    if (cos_n1_r12 <= nr_max && -cos_n2_r12 <= nr_max &&
        p.norm <= ss_ljs[types[p.i1]][types[p.i2]].sink_max) {

        diff.cont.type = QAContact::Type::SS;
        return true;
    }

    return false;
}

void QuasiAdiabatic::forwardStatDiff(int i1, int i2, QAContact const& cont,
    Stat *diffs) const {

    diffs[0] = diffs[1] = Stat();
    int idx[2] = { i1, i2 };

    if (cont.type == QAContact::Type::BB ||
        cont.type == QAContact::Type::BS) {

        ++diffs[0].backbone;
    }
    else {
        ++diffs[0].sidechain;
    }

    if (cont.type == QAContact::Type::BB ||
        cont.type == QAContact::Type::SB) {

        ++diffs[0].backbone;
    }
    else {
        ++diffs[0].sidechain;
    }

    if (cont.type == QAContact::Type::SS) {
        for (int k = 0; k < 2; ++k) {
            auto other = idx[1 - k];
            auto const& other_spec = specs[types[other]];

            switch (other_spec.polarization) {
            case Polarization::POLAR:
            case Polarization::POLAR_NEG:
            case Polarization::POLAR_POS:
                ++diffs[k].polarSS;
                break;
            case Polarization::HYDROPHOBIC:
                ++diffs[k].hydrophobicSS;
                break;
            default:
                break;
            }
        }
    }
}

bool QuasiAdiabatic::checkStat(int i, const Stat &stat) const {
    auto const& spec = specs[types[i]];

    if (stat.backbone > 2)
        return false;

    if (types[i] == (int8_t)AAType::PRO && stat.backbone > 1)
        return false;

    if (stat.sidechain > spec.maxSidechain)
        return false;

    if (stat.hydrophobicSS > spec.maxHydrophobicSS)
        return false;

    if (stat.polarSS > spec.maxPolarSS)
        return false;

    return true;
}

bool QuasiAdiabatic::coordinationPhase(PairInfo const& p, const BaseState &state,
    QADiff &diff) const {

    forwardStatDiff(p.i1, p.i2, diff.cont, diff.statDiffs);

    Stat total[2];
    total[0] = state.qaState.stats[p.i1] + diff.statDiffs[0];
    total[1] = state.qaState.stats[p.i2] + diff.statDiffs[1];

    return checkStat(p.i1, total[0]) && checkStat(p.i2, total[1]);
}

void QuasiAdiabatic::tryForming(PairInfo const& p, BaseState const& state,
    BaseDiff& diff) const {

    QADiff qaDiff;
    if (!geometryPhase(p, qaDiff)) return;
    if (!coordinationPhase(p, state, qaDiff)) return;

    qaDiff.cont = (QAContact) {
        .i1 = p.i1, .i2 = p.i2,
        .status = QAContact::Status::FORMING,
        .t0 = state.t
    };
    diff.qaDiffs.emplace_back(qaDiff);
}

vl::Spec QuasiAdiabatic::recomputeSpec() const {
    double maxCutoff = 0.0;
    maxCutoff = std::max(maxCutoff, bb_lj.cutoff());
    maxCutoff = std::max(maxCutoff, bs_lj.cutoff());

    for (int8_t acid1 = 0; acid1 < AminoAcid::N; ++acid1) {
        for (int8_t acid2 = 0; acid2 < AminoAcid::N; ++acid2) {
            maxCutoff = std::max(maxCutoff, ss_ljs[acid1][acid2].cutoff());
        }
    }

    return (vl::Spec) {
        .cutoffSq = pow(maxCutoff, 2.0),
        .minBondSep = 3,
        .destructive = false,
    };
}

void QuasiAdiabatic::tryBreaking(PairInfo const& p, BaseState const& state,
    QAContact const& cont, BaseDiff& diff) const {

    double r_min = 0.0;
    switch (cont.type) {
    case QAContact::Type::BB:
        r_min = bb_lj.r_min;
        break;
    case QAContact::Type::BS:
    case QAContact::Type::SB:
        r_min = bs_lj.r_min;
        break;
    case QAContact::Type::SS:
        r_min = ss_ljs[types[cont.i1]][types[cont.i2]].sink_max;
        break;
    };

    if (p.norm > breakingTolerance * pow(2.0, -1.0/6.0) * r_min) {
        QADiff qaDiff;
        qaDiff.cont = cont;
        qaDiff.cont.status = QAContact::Status::BREAKING;
        qaDiff.cont.t0 = state.t;
        qaDiff.request = QADiff::Request::BREAK;

        diff.qaDiffs.emplace_back(qaDiff);
    }
}

void QuasiAdiabatic::destroy(QAContact &cont, BaseDiff &diff) const {
    QADiff qaDiff;
    qaDiff.cont = cont;
    forwardStatDiff(cont.i1, cont.i2, cont, qaDiff.statDiffs);
    qaDiff.statDiffs[0] = -qaDiff.statDiffs[0];
    qaDiff.statDiffs[1] = -qaDiff.statDiffs[1];
    qaDiff.request = QADiff::Request::DESTROY;

    diff.qaDiffs.emplace_back(qaDiff);
}

bool operator<(QAContact const& p1, std::pair<int, int> const& p2) {
    return std::make_pair(p1.i1, p1.i2) < p2;
}

void QuasiAdiabatic::onVLUpdate(Pairs &vl, BaseState& state) {
    std::swap(oldQAContacts, state.qaState.pairs);

    state.qaState.pairs.clear();
    std::set_intersection(
        oldQAContacts.begin(), oldQAContacts.end(),
        vl.begin(), vl.end(),
        std::back_inserter(state.qaState.pairs));

    freePairs.clear();
}