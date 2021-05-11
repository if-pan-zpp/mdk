#include "cpu/nonlocal/QuasiAdiabatic.hpp"
#include "cpu/kernels/LennardJones.hpp"
using namespace mdk;
using namespace mdk::param;

QuasiAdiabatic::QuasiAdiabatic(Model const& model, const Parameters &params) {
    types = Types(model);
    seqs = Sequences(model);

    for (auto const& acid: AminoAcid::aminoAcids()) {
        auto const& spec = params.specificity.at(acid);
        specs[(int8_t)acid] = spec;
    }

    bb_lj.r_min = 5.0 * angstrom;
    bs_lj.r_min = 6.8 * angstrom;
    bb_lj.depth = bs_lj.depth = 1.0 * eps;

    for (auto const& [acids, r]: params.pairwiseMinDist) {
        auto acid1 = (int8_t)acids.first, acid2 = (int8_t)acids.second;
        ss_ljs[acid1][acid2].sink_max = ss_ljs[acid2][acid1].sink_max = r;
    }


    n = h = Vectors(model.n);
}

bool QuasiAdiabatic::geometryPhase(const vl::Free &p, QAContact &cont) const {
    Vector h1 = h[p.i1], h2 = h[p.i2];
    double cos_h1_r12 = h1.dot(p.unit), cos_h2_r12 = h2.dot(p.unit),
        cos_h1_h2 = h1.dot(h2);

    if (abs(cos_h1_r12) >= hr_min && abs(cos_h2_r12) >= hr_min &&
        abs(cos_h1_h2) >= hh_min && p.norm <= bb_lj.r_min) {

        cont.type = QAContact::Type::BB;
        return true;
    }

    Vector n1 = n[p.i1];
    double cos_n1_r12 = n1.dot(p.unit);
    if (cos_n1_r12 <= nr_max && abs(cos_h1_r12) >= hr_min &&
        p.norm <= bs_lj.r_min) {

        cont.type = QAContact::Type::BS;
        return true;
    }

    Vector n2 = n[p.i2];
    double cos_n2_r12 = n2.dot(p.unit);
    if (cos_n2_r12 <= nr_max && abs(cos_h1_r12) >= hr_min &&
        p.norm <= bs_lj.r_min) {

        cont.type = QAContact::Type::SB;
        return true;
    }

    if (cos_n1_r12 <= nr_max && -cos_n2_r12 <= nr_max &&
        p.norm <= ss_ljs[types[p.i1]][types[p.i2]].sink_max) {

        cont.type = QAContact::Type::SS;
        return true;
    }

    return false;
}

void QuasiAdiabatic::forwardStatDiff(const vl::Base &p, const QAContact &cont,
    Stat *diffs) const {

    diffs[0] = diffs[1] = Stat();
    int idx[2] = { p.i1, p.i2 };

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

bool QuasiAdiabatic::coordinationPhase(const vl::Base &p, const State &state,
        QAContact &cont, Stat stats[2]) const {

    Stat diffs[2];
    forwardStatDiff(p, cont, diffs);

    stats[0] = state.stats[p.i1] + diffs[0];
    stats[1] = state.stats[p.i2] + diffs[1];

    int idx[2] = { p.i1, p.i2 };
    for (int k = 0; k < 2; ++k) {
        auto cur = idx[k];
        auto const& spec = specs[types[cur]];

        if (stats[k].backbone > 2)
            return false;

        if (types[cur] == (int8_t)AAType::PRO && stats[k].backbone > 1)
            return false;

        if (stats[k].sidechain > spec.maxSidechain)
            return false;

        if (stats[k].hydrophobicSS > spec.maxHydrophobicSS)
            return false;

        if (stats[k].polarSS > spec.maxPolarSS)
            return false;
    }

    return true;
}



void QuasiAdiabatic::tryBreaking(QAContact &cont, State const& state) const {
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

    if (cont.norm > breakingTolerance * pow(2.0, -1.0/6.0) * r_min) {
        cont.status = QAContact::Status::BREAKING;
        cont.time0 = state.t;
    }
}

void QuasiAdiabatic::tryForming(vl::Free& p, State &state) const {
    QAContact cont;

    if (seqs.isTerminal[p.i1] || seqs.isTerminal[p.i2])
        return;

    if (!include4 && seqs.sepByN(p.i1, p.i2, 4))
        return;

    if (!geometryPhase(p, cont)) return;

    Stat stats[2];
    if (!coordinationPhase(p, state, cont, stats)) return;

    cont.status = QAContact::Status::FORMING;
    cont.time0 = state.t;
    p.status = vl::Free::Status::TAKEN;

    state.stats[p.i1] = stats[0];
    state.stats[p.i2] = stats[1];
}

void QuasiAdiabatic::destroy(QAContact &cont, State &state) const {
    Stat diffs[2];
    forwardStatDiff((vl::Base&)cont, cont, diffs);

    state.stats[cont.i1] -= diffs[0];
    state.stats[cont.i2] -= diffs[1];
    cont.status = QAContact::Status::VACATED;
}

double QuasiAdiabatic::cutoff() const {
    double maxCutoff = 0.0;
    maxCutoff = std::max(maxCutoff, bb_lj.cutoff());
    maxCutoff = std::max(maxCutoff, bs_lj.cutoff());

    for (int8_t acid1 = 0; acid1 < AminoAcid::N; ++acid1) {
        for (int8_t acid2 = 0; acid2 < AminoAcid::N; ++acid2) {
            maxCutoff = std::max(maxCutoff, ss_ljs[acid1][acid2].cutoff());
        }
    }

    return maxCutoff;
}

void QuasiAdiabatic::perPair(QAContact& cont, State &state,
    Dynamics &dyn) const {

    if (cont.status == QAContact::Status::VACATED)
        return;

    double stage;
    if (cont.status == QAContact::Status::FORMING) {
        stage = std::min(1.0, (state.t - cont.time0) / formationTime);
    }
    else {
        stage = std::max(0.0, 1.0 - (state.t - cont.time0) / breakingTime);
    }

    if (cont.type != QAContact::Type::SS) {
        auto lj = cont.type == QAContact::Type::BB ? bb_lj : bs_lj;
        lj.depth *= stage;
        lj.asForce(cont.unit, cont.norm,
            dyn.V, dyn.F[cont.i1], dyn.F[cont.i2]);
    }
    else {
        auto ssLJ = ss_ljs[types[cont.i1]][types[cont.i2]];
        ssLJ.depth *= stage;
        ssLJ.asForce(cont.unit, cont.norm,
            dyn.V, dyn.F[cont.i1], dyn.F[cont.i2]);
    }

    if (cont.status == QAContact::Status::FORMING && stage >= 1.0) {
        tryBreaking(cont, state);
    }
    else if (cont.status == QAContact::Status::BREAKING && stage <= 0.0) {
        destroy(cont, state);
    }
}