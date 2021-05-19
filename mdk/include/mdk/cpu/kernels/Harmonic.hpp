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

        inline double cutoff() const {
            return 0.0;
        }

        inline void kernel(double diff, double& V, double& dV_dn) const {
            auto diff2 = diff*diff;
            V += diff2 * (H1 + H2 * diff);
            dV_dn += diff * (2.0 * H1 + 4.0 * H2 * diff2);
        }

        template<typename T1, typename T2>
        inline void asForce(VRef unit, double diff, double& V,
            T1 F1, T2 F2) const {

            double dV_dn = 0.0;
            kernel(diff, V, dV_dn);
            F1 += dV_dn * unit;
            F2 -= dV_dn * unit;
        }
    };
}
