#pragma once
#include <mdk/cpu/data/State.hpp>

namespace mdk {
    class Chirality {
    private:
        Scalars d0_cube_invs, C_nats;

    public:
        double e_chi = 1.0 * eps;

        explicit Chirality(Model const& model);

        void perQuad(int i, VRef r12, Vector r12_x_r23, Vector r12_x_r34,
            Vector r23_x_r34, double &V, Vectors &dV_dr) const;
    };

    inline void Chirality::perQuad(int i, VRef r12, Vector r12_x_r23,
            Vector r12_x_r34, Vector r23_x_r34, double &V, Vectors &dV_dr) const {
        auto i1 = i-2, i2 = i-1, i3 = i, i4 = i+1;

        auto d0_cube_inv = d0_cube_invs[i];
        auto C_nat = C_nats[i];

        auto C = r12.dot(r23_x_r34) * d0_cube_inv;
        V += 0.5 * e_chi * pow(C - C_nat, 2);

        auto f = e_chi * (C - C_nat) * d0_cube_inv;
        r12_x_r23 *= f;
        r23_x_r34 *= f;
        r12_x_r34 *= f;

        dV_dr[i1] -= r23_x_r34;
        dV_dr[i2] += r12_x_r34 + r23_x_r34;
        dV_dr[i3] -= r12_x_r23 + r12_x_r34;
        dV_dr[i4] += r12_x_r23;
    }
}
