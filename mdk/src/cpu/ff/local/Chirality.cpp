#include "cpu/ff/local/Chirality.hpp"
using namespace mdk;

Chirality::Chirality(const Model &model) {
    d0_cube_invs = Scalars(model.n);
    C_nats = Scalars(model.n);

    for (auto const& [start, end]: model.chains) {
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