#pragma once
#include <mdk/cpu/simul/State.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>

namespace mdk {
    class Chirality {
    private:
        Scalars d0_cube_invs, C_nats;

    public:
        double e_chi = 1.0 * eps;

        explicit Chirality(Model const& model);

        inline void perQuad(int i, VRef r12, Vector r12_x_r23, Vector r12_x_r34,
            Vector r23_x_r34, Dynamics &dyn) const {

            auto i1 = i-2, i2 = i-1, i3 = i, i4 = i+1;

            auto d0_cube_inv = d0_cube_invs[i];
            auto C_nat = C_nats[i];

            auto C = r12.dot(r23_x_r34) * d0_cube_inv;
            dyn.V += 0.5 * e_chi * pow(C - C_nat, 2);

            auto f = e_chi * (C - C_nat) * d0_cube_inv;
            r12_x_r23 *= f;
            r23_x_r34 *= f;
            r12_x_r34 *= f;

            dyn.F[i1] += r23_x_r34;
            dyn.F[i2] -= r12_x_r34 + r23_x_r34;
            dyn.F[i3] += r12_x_r23 + r12_x_r34;
            dyn.F[i4] -= r12_x_r23;
        }
    };
}
