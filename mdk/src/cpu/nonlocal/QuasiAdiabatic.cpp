#include "cpu/nonlocal/QuasiAdiabatic.hpp"
#include "cpu/generic/LennardJones.hpp"
using namespace mdk;
using namespace mdk::param;

QuasiAdiabatic::QuasiAdiabatic(Model const& model,
        const Parameters &params) {
    types = Types(model);

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

bool QuasiAdiabatic::directionPhase(const vl::Base &p, const State &state,
    qa::Contact &cont) const {

    Vector h1 = h[p.i1], h2 = h[p.i2];
    double cos_h1_r12 = h1.dot(p.unit), cos_h2_r12 = h2.dot(p.unit),
        cos_h1_h2 = h1.dot(h2);
    if (abs(cos_h1_r12) >= hr_min && abs(cos_h2_r12) >= hr_min &&
        abs(cos_h1_h2) >= hh_min) {
        cont.type = qa::Type::BB;
        return true;
    }

    Vector n1 = n[p.i1];
    double cos_n1_r12 = n1.dot(p.unit);
    if (cos_n1_r12 <= nr_max && abs(cos_h1_r12) >= hr_min) {
        cont.type = qa::Type::BS;
        return true;
    }

    Vector n2 = n[p.i2];
    double cos_n2_r12 = n2.dot(p.unit);
    if (cos_n2_r12 <= nr_max && abs(cos_h1_r12) >= hr_min) {
        cont.type = qa::Type::SB;
        return true;
    }

    if (cos_n1_r12 <= nr_max && -cos_n2_r12 <= nr_max) {
        cont.type = qa::Type::SS;
        return true;
    }

    return false;
}

bool QuasiAdiabatic::distancePhase(const vl::Base &p, const State &state,
    qa::Contact &cont) const {

    if (cont.type == qa::Type::BB) {
        cont.r_min = bb_cutoff;
    }
    else if (cont.type == qa::Type::SS) {
        cont.r_min = bb_cutoff;
    }
    else {
        auto type1 = (int8_t)types[p.i1], type2 = (int8_t)types[p.i2];
        cont.r_min = ss_cutoff[type1][type2];
    }

    double min_dist = (1.0 + formationTolerance) * cont.r_min;
    return p.norm <= min_dist;
}

void QuasiAdiabatic::forwardStatDiff(const vl::Base &p, const qa::Contact &cont,
    Stat *diffs) const {

    diffs[0] = diffs[1] = Stat();
    int idx[2] = { p.i1, p.i2 };

    if (cont.type == qa::Type::BB || cont.type == qa::Type::BS) {
        ++diffs[0].backbone;
    }
    else {
        ++diffs[0].sidechain;
    }

    if (cont.type == qa::Type::BB || cont.type == qa::Type::SB) {
        ++diffs[0].backbone;
    }
    else {
        ++diffs[0].sidechain;
    }

    if (cont.type == qa::Type::SS) {
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
        qa::Contact &cont, Stat stats[2]) const {

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

void QuasiAdiabatic::tryBreaking(const vl::Base &p, State const& state,
    qa::Contact &cont) const {

    if (p.norm > breakingTolerance * pow(2.0, -1.0/6.0) * cont.r_min) {
        cont.status = qa::Status::BREAKING;
        cont.time0 = state.t;
    }
}

void QuasiAdiabatic::tryForming(SystemVLItem &item, State &state) const {

    qa::Contact cont;
    vl::Base const& p = item.base();
    if (!directionPhase(p, state, cont)) return;
    if (!distancePhase(p, state, cont)) return;

    Stat stats[2];
    if (!coordinationPhase(p, state, cont, stats)) return;

    cont.status = qa::Status::FORMING;
    cont.time0 = state.t;
    item.template morph<qa::Contact>(cont);

    state.stats[p.i1] = stats[0];
    state.stats[p.i2] = stats[1];
}

void QuasiAdiabatic::destroy(SystemVLItem &item, State &state) const {
    Stat diffs[2];
    vl::Base const& p = item.base();
    qa::Contact& cont = item.template data<qa::Contact>();
    forwardStatDiff(p, cont, diffs);

    state.stats[p.i1] -= diffs[0];
    state.stats[p.i2] -= diffs[1];
    item.template morph<NoContact>();
}

void QuasiAdiabatic::perPair(SystemVLItem &item, State &state,
    Dynamics &dyn) const {

    thread_local LennardJones ljV;
    vl::Base const& p = item.base();
    qa::Contact& cont = item.template data<qa::Contact>();

    if (cont.status == qa::Status::FORMING) {
        ljV.r_min = cont.r_min;
        double stage = std::min(1.0, (state.t - cont.time0) / formationTime);
        ljV.depth = stage * maxDepth;
        ljV.asForce(p.unit, p.norm,
            dyn.V, dyn.dV_dr[p.i1], dyn.dV_dr[p.i2]);
    }
    else {
        double stage = std::max(0.0, (state.t - cont.time0) / breakingTime);
        if (stage > 0.0) {
            ljV.r_min = cont.r_min;
            ljV.depth = maxDepth * stage;
            ljV.asForce(p.unit, p.norm,
                dyn.V, dyn.dV_dr[p.i1], dyn.dV_dr[p.i2]);
        }
        else {
            destroy(item, state);
            return;
        }
    }

    tryBreaking(p, state, cont);
}