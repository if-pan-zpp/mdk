#pragma once
#include <mdk/utils/Units.hpp>
#include <mdk/cpu/data/Primitives.hpp>

namespace mdk {
    class LennardJones {
    public:
        double r_min = 5.0*angstrom, depth = 1.0*eps;

        LennardJones() = default;
        LennardJones(double r_min, double depth):
            r_min(r_min), depth(depth) {};

        inline double cutoff() const {
            return 2.5 * pow(2.0, -1.0/6.0) * r_min;
        }

        inline void kernel(double norm, double& V, double& dV_dn) const {
            auto norm_inv = 1.0 / norm, s = norm_inv * r_min;
            auto s6 = s*s*s*s*s*s, s12 = s6*s6;
            V += depth * (s12 - 2.0 * s6);
            dV_dn += 12 * depth * (s6 - s12) * norm_inv;
        }

        template<typename T1, typename T2>
        inline void asForce(VRef unit, double norm, double& V,
            T1 F1, T2 F2) const {

            double dV_dn = 0.0;
            kernel(norm, V, dV_dn);
            F1 -= dV_dn * unit;
            F2 += dV_dn * unit;
        }
    };
}
