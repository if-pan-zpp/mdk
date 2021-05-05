#include "cpu/qa/QuasiAdiabatic.hpp"
#include "cpu/generic/LennardJones.hpp"
using namespace mdk;
using namespace mdk::qa;

QuasiAdiabatic::QuasiAdiabatic(Model const& model,
        const Parameters &params) {
    types = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>(model.n, -1);
    for (int i = 0; i < model.n; ++i) {
        types[i] = (int8_t)model.residues[i].type;
    }

    for (auto const& acid: AminoAcid::aminoAcids()) {
        auto const& spec = params.specificity.at(acid);
        specs[(int8_t)acid] = spec;
    }

    for (auto const& [acids, r]: params.pairwiseMinDist) {
        auto acid1 = (int8_t)acids.first, acid2 = (int8_t)acids.second;
        ss_cutoff[acid1][acid2] = ss_cutoff[acid2][acid1] = r;
    }

    n = h = Vectors(model.n);
}

bool QuasiAdiabatic::dirPhase(vl::Pair<NormalData>& p,
        VRef unit12) const {
    Vector h1 = h[p.i1], h2 = h[p.i2];
    double cos_h1_r12 = h1.dot(unit12), cos_h2_r12 = h2.dot(unit12),
        cos_h1_h2 = h1.dot(h2);
    if (abs(cos_h1_r12) >= hr_min && abs(cos_h2_r12) >= hr_min &&
            abs(cos_h1_h2) >= hh_min) {
        p.data.qaType = qa::ContactType::BB;
        return true;
    }

    Vector n1 = n[p.i1];
    double cos_n1_r12 = n1.dot(unit12);
    if (cos_n1_r12 <= nr_max && abs(cos_h1_r12) >= hr_min) {
        p.data.qaType = qa::ContactType::BS;
        return true;
    }

    Vector n2 = n[p.i2];
    double cos_n2_r12 = n2.dot(unit12);
    if (cos_n2_r12 <= nr_max && abs(cos_h1_r12) >= hr_min) {
        p.data.qaType = qa::ContactType::SB;
        return true;
    }

    if (cos_n1_r12 <= nr_max && -cos_n2_r12 <= nr_max) {
        p.data.qaType = qa::ContactType::SS;
        return true;
    }

    return false;
}

bool QuasiAdiabatic::distPhase(vl::Pair<NormalData>& p, double norm) const {
    if (p.data.qaType == qa::ContactType::BB) {
        p.data.r_min = bb_cutoff;
    }
    else if (p.data.qaType != qa::ContactType::SS) {
        p.data.r_min = bs_cutoff;
    }
    else {
        p.data.r_min = ss_cutoff[types[p.i1]][types[p.i2]];
    }

    double min_dist = (1.0 + formationTolerance) * p.data.r_min;
    return norm <= min_dist;
}

void QuasiAdiabatic::adjustStats(const vl::Pair<NormalData> &p,
        qa::Stat *stat, int dir) const {
    int idx[2] = { p.i1, p.i2 };

    if (p.data.qaType == qa::ContactType::BB ||
        p.data.qaType == qa::ContactType::BS) {
        stat[0].backbone += dir;
    }
    else {
        stat[0].sidechain += dir;
    }

    if (p.data.qaType == qa::ContactType::BB ||
        p.data.qaType == qa::ContactType::SB) {
        stat[1].backbone += dir;
    }
    else {
        stat[1].sidechain += dir;
    }

    if (p.data.qaType == qa::ContactType::SS) {
        for (int k = 0; k < 2; ++k) {
            auto other = idx[1 - k];
            auto const& other_spec = specs[types[other]];

            switch (other_spec.polarization) {
            case Polarization::POLAR:
            case Polarization::POLAR_NEG:
            case Polarization::POLAR_POS:
                ++stat[k].polarSS;
                break;
            case Polarization::HYDROPHOBIC:
                ++stat[k].hydrophobicSS;
                break;
            default:
                break;
            }
        }
    }
}

bool QuasiAdiabatic::coordPhase(vl::Pair<NormalData>& p, qa::Stats const& stats,
    qa::Diff& diff) const {

    diff.stats[0] = stats[p.i1];
    diff.stats[1] = stats[p.i2];
    adjustStats(p, diff.stats, +1);

    int idx[2] = { p.i1, p.i2 };
    for (int k = 0; k < 2; ++k) {
        auto cur = idx[k];
        auto const& spec = specs[types[cur]];

        if (diff.stats[k].backbone > 2)
            return false;

        if (types[cur] == (int8_t)AAType::PRO && diff.stats[k].backbone > 1)
            return false;

        if (diff.stats[k].sidechain > spec.maxSidechain)
            return false;

        if (diff.stats[k].hydrophobicSS > spec.maxHydrophobicSS)
            return false;

        if (diff.stats[k].polarSS > spec.maxPolarSS)
            return false;
    }

    return true;
}

void QuasiAdiabatic::tryCreate(vl::Pair<NormalData> const& p, VRef unit12, double norm,
    State const& state, StateDiff& sd) const {

    vl::Pair<NormalData> qaPair = p;
    if (!dirPhase(qaPair, unit12)) return;
    if (!distPhase(qaPair, norm)) return;

    qa::Diff diff;
    if (!coordPhase(qaPair, state.qaStats, diff)) return;

    qaPair.data.time0 = state.t;
    sd.qaDiffs.emplace_back(diff);
}

void QuasiAdiabatic::tryDestroy(const vl::Pair<NormalData> &p, double norm,
        const State &state, StateDiff &sd) const {

    double maxNorm = breakingTolerance * pow(2.0, -1.0/6.0) * p.data.r_min;
    if (norm > maxNorm) {
        qa::Diff diff;
        diff.pair = p;
        diff.pair.data.type = NCType::QA_BREAKING;
        diff.pair.data.time0 = state.t;

        sd.qaDiffs.emplace_back(diff);
    }
}

void QuasiAdiabatic::removePair(const vl::Pair<NormalData> &p,
        const State &state, StateDiff &sd) const {
    qa::Diff diff;
    diff.pair = p;
    diff.pair.data.type = NCType::NONE;

    diff.stats[0] = state.qaStats[p.i1];
    diff.stats[1] = state.qaStats[p.i2];
    adjustStats(p, diff.stats, -1);

    sd.qaDiffs.emplace_back(diff);
}

void QuasiAdiabatic::perPair(const vl::Pair<NormalData> &p, VRef unit12,
    double norm, State const& state, StateDiff& sd) const {

    if (p.data.type == NCType::NONE) {
        tryCreate(p, unit12, norm, state, sd);
    }
    else if (p.data.type == NCType::QA_FORMING || p.data.type == NCType::QA_BREAKING) {
        tryDestroy(p, norm, state, sd);

        LennardJones ljV;
        if (p.data.type == NCType::QA_FORMING) {
            ljV.rmin = p.data.r_min;
            double stage = std::min(1.0, (state.t - p.data.time0) / formationTime);
            ljV.depth = maxDepth * stage;
            ljV.eval(unit12, norm, sd.V, sd.dV_dr[p.i1], sd.dV_dr[p.i2]);
        }
        else {
            ljV.rmin = p.data.r_min;
            double stage = std::max(0.0, (p.data.time0 - state.t) / breakingTime);
            if (stage > 0.0) {
                ljV.depth = maxDepth * stage;
                ljV.eval(unit12, norm, sd.V, sd.dV_dr[p.i1], sd.dV_dr[p.i2]);
            }
            else {
                removePair(p, state, sd);
            }
        }
    }
}
