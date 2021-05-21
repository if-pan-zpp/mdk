#pragma once
#include <mdk/utils/Units.hpp>
#include <mdk/cpu/data/Primitives.hpp>

namespace mdk {
    class Harmonic {
    public:
        double H1 = 50.0 * eps/pow(angstrom, 2.0);
        double H2 = 0.0;

        Harmonic() = default;
        Harmonic(double H1, double H2):
            H1(H1), H2(H2) {};

        inline void computeV(double dx, double& V, double& dV_dx) const {
            auto dx2 = dx*dx;
            V += dx2 * (H1 + H2 * dx);
            dV_dx += dx * (2.0 * H1 + 4.0 * H2 * dx2);
        }

        template<typename T1, typename T2>
        inline void computeF(VRef unit, double dx, double& V,
            T1 F1, T2 F2) const {

            double dV_dn = 0.0;
            computeV(dx, V, dV_dn);
            F1 += dV_dn * unit;
            F2 -= dV_dn * unit;
        }
    };
}
