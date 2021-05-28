#pragma once
#include "../data/Primitives.hpp"
#include "../utils/Units.hpp"

namespace mdk {
    class ShiftedTruncatedLJ {
    public:
        double r_cut = 4.0 * angstrom, depth = 1.0 * eps;

        ShiftedTruncatedLJ()  = default;
        ShiftedTruncatedLJ(double r_cut, double depth):
            r_cut(r_cut), depth(depth) {};

        inline double cutoff() const {
            return r_cut;
        }

        inline void kernel(double norm, double& V, double& dV_dn) const {
            auto norm_inv = 1.0 / norm, s = norm_inv * r_cut;
            auto s6 = s*s*s*s*s*s, s12 = s6*s6;
            V += depth * (s12 - 2.0 * s6 + 1.0);
            dV_dn +=  12 * depth * (s6 - s12) * norm_inv;
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
