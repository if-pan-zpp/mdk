#include "forces/Chirality.hpp"
using namespace mdk;

Chirality::Chirality(const Model &model) {
    d0_cube_inv = Scalars(model.n);
    C_nat = Scalars(model.n);

    for (auto const& chain: model.chains) {
        auto start = chain.start + 2, end = chain.end - 1;
        ranges.emplace_back(start, end);

        for (int i = start; i < end; ++i) {
            auto i1 = i-2, i2 = i-1, i3 = i, i4 = i+1;
            auto r1 = model.residues[i1].pos,
                 r2 = model.residues[i2].pos,
                 r3 = model.residues[i3].pos,
                 r4 = model.residues[i4].pos;

            auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
            auto r23_x_r34 = r23.cross(r34);

            auto d0_cube_inv_i = pow(r23.norm(), -3.0);
            auto C_nat_i = r12.dot(r23_x_r34) * d0_cube_inv_i;

            d0_cube_inv[i] = d0_cube_inv_i;
            C_nat[i] = C_nat_i;
        }
    }
}

void Chirality::eval(const BaseState &state, BaseDiff &update) const {
    for (auto const& [start, end]: ranges) {
        for (int i = start; i < end; ++i) {
            auto r1 = state.r[i-2], r2 = state.r[i-1], r3 = state.r[i],
                 r4 = state.r[i+1];
            auto r12 = r2 - r1, r23 = r3 - r2, r34 = r4 - r3;
            auto r12_x_r23 = r12.cross(r23), r12_x_r34 = r12.cross(r34),
                 r23_x_r34 = r23.cross(r34);

            auto C = r12.dot(r23_x_r34) * d0_cube_inv[i];
            auto diffC = C - C_nat[i];
            update.V += 0.5 * e_chi * diffC * diffC;

            auto f = e_chi * diffC * d0_cube_inv[i];
            update.F[i-2] += f * r23_x_r34;
            update.F[i-1] -= f * (r12_x_r34 + r23_x_r34);
            update.F[i] += f * (r12_x_r23 + r12_x_r34);
            update.F[i+1] -= r12_x_r23;
        }
    }
}
