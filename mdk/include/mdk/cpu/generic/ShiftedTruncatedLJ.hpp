#pragma once
#include <mdk/cpu/data/Primitives.hpp>

namespace mdk {
    class ShiftedTruncatedLJ {
    public:
        double r_min = 4.0 * angstrom, depth = 1.0 * eps;
        double cutoff2;

        mutable double _cutoff;
        inline double cutoff() const {
            _cutoff = sqrt(cutoff2);
            return _cutoff;
        }

        inline void kernel(double norm, double& V, double& dV_dn) const {
            auto norm_inv = 1.0 / norm, s = norm_inv * r_min;
            auto s6 = s*s*s*s*s*s, s12 = s6*s6;
            V += depth * (s12 - 2.0 * s6 + 1.0);
            dV_dn +=  depth * (s6 - s12) * norm_inv;
        }

        inline void asForce(VRef unit, double norm, double& V,
            Vector& F1, Vector& F2) const {

            double dV_dn = 0.0;
            kernel(norm, V, dV_dn);
            F1 -= dV_dn * unit;
            F2 += dV_dn * unit;
        }
    };
}