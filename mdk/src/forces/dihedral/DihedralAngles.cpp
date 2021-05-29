#include "forces/dihedral/DihedralAngles.hpp"
using namespace mdk;

void DihedralAngles::bind(Simulation &simulation) {
    Force::bind(simulation);
    inRange =  Bytes(state->n, false);

    auto const& model = simulation.data<Model>();
    for (auto const& ch: model.chains) {
        auto start = ch.start + 2, end = ch.end - 1;
        for (int i = start; i < end; ++i)
            inRange[i] = true;
    }
}

void DihedralAngles::asyncPart() {
    for (int i = 0; i < inRange.size(); ++i) {
        if (!inRange[i]) continue;

        auto r1 = state->r[i-2], r2 = state->r[i-1],
            r3 = state->r[i],   r4 = state->r[i+1];
        auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
        auto r23_norm = r23.norm();

        auto r12_x_r23 = r12.cross(r23), r23_x_r34 = r23.cross(r34);
        auto r12_x_r23_normsq = r12_x_r23.squaredNorm();
        auto r23_x_r34_normsq = r23_x_r34.squaredNorm();

        if (r12_x_r23_normsq != 0.0 && r23_x_r34_normsq != 0.0) {
            auto r12_x_r23_norm = sqrt(r12_x_r23_normsq);
            auto unit_r12_x_r23 = r12_x_r23 / r12_x_r23_norm;

            auto r23_x_r34_norm = sqrt(r23_x_r34_normsq);
            auto unit_r23_x_r34 = r23_x_r34 / r23_x_r34_norm;

            auto cos_phi = unit_r12_x_r23.dot(unit_r23_x_r34);
            cos_phi = std::max(std::min(cos_phi, 1.0), -1.0);
            auto phi = acos(cos_phi), dV_dphi = 0.0;
            if (r12_x_r23.dot(r34) < 0.0) phi = -phi;

            if (compNatDih && compNatDih->isNative[i]) {
                compNatDih->term(i, phi, state->dyn.V, dV_dphi);
            }
            else if (simpNatDih && simpNatDih->isNative[i]) {
                simpNatDih->term(i, phi, state->dyn.V, dV_dphi);
            }
            else if (heurDih) {
                heurDih->term(i, phi, state->dyn.V, dV_dphi);
            }

            auto dphi_dr1 = -unit_r12_x_r23 * r23_norm / r12_x_r23_norm;
            auto dphi_dr4 = unit_r23_x_r34 * r23_norm / r23_x_r34_norm;
            Vector df = (-dphi_dr1*r12.dot(r23)+dphi_dr4*r23.dot(r34));
            df /= (r23_norm * r23_norm);
            auto dphi_dr2 = -dphi_dr1 + df;
            auto dphi_dr3 = -dphi_dr4 - df;

            state->dyn.F[i-2] -= dV_dphi * dphi_dr1;
            state->dyn.F[i-1] -= dV_dphi * dphi_dr2;
            state->dyn.F[i] -= dV_dphi * dphi_dr3;
            state->dyn.F[i+1] -= dV_dphi * dphi_dr4;
        }
    }
}