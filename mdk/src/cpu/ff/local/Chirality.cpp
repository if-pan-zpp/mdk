#include "cpu/ff/Chirality.hpp"
using namespace mdk;

Chirality::Chirality(const Model &model) {
    d0_cube_invs = Scalars(model.n);
    C_nats = Scalars(model.n);

    for (auto const& [start, end]: model.chains) {
        regions.emplace_back(start+2, end-1);
        for (int i = start+2; i < end-1; ++i) {
            auto i1 = i-2, i2 = i-1, i3 = i, i4 = i+1;
            auto r1 = model.residues[i1].pos,
                 r2 = model.residues[i2].pos,
                 r3 = model.residues[i3].pos,
                 r4 = model.residues[i4].pos;

            auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
            auto r12_x_r23 = r12.cross(r23), r12_x_r34 = r12.cross(r34),
                 r23_x_r34 = r23.cross(r34);

            auto d0_cube_inv = pow(r23.norm(), -3.0);
            auto C_nat = r12.dot(r23_x_r34) * d0_cube_inv;

            d0_cube_invs[i] = d0_cube_inv;
            C_nats[i] = C_nat;
        }
    }
}

void Chirality::compute(const State &state, double &V, Vectors &F) {
    for (auto const& [start, end]: regions) {
        for (int i = start; i < end; ++i) {
            auto i1 = i-2, i2 = i-1, i3 = i, i4 = i+1;
            auto r1 = state.r[i1], r2 = state.r[i2], r3 = state.r[i3],
                 r4 = state.r[i4];

            auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
            auto r12_x_r23 = r12.cross(r23), r12_x_r34 = r12.cross(r34),
                 r23_x_r34 = r23.cross(r34);

            auto d0_cube_inv = d0_cube_invs[i];
            auto C_nat = C_nats[i];

            auto C = r12.dot(r23_x_r34) * d0_cube_inv;
            V += 0.5 * e_chi * pow(C - C_nat, 2);

            auto f = e_chi * (C - C_nat) * d0_cube_inv;
            r12_x_r23 *= f;
            r23_x_r34 *= f;
            r12_x_r34 *= f;

            F[i1] += r23_x_r34;
            F[i2] -= r12_x_r34 + r23_x_r34;
            F[i3] += r12_x_r23 + r12_x_r34;
            F[i4] -= r12_x_r23;
        }
    }
}