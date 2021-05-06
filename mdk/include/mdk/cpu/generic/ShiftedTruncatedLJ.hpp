#pragma once

namespace mdk {
    class ShiftedTruncatedLJ {
    public:
        double r_min, depth;
        double cutoff2;

        void kernel(double norm, double& V, double& dV_dn) const;
        void asForce(VRef unit, double norm, double& V,
            Vector& dV_dr1, Vector& dV_dr2) const;
    };

    inline void ShiftedTruncatedLJ::kernel(double norm, double& V, double& dV_dn) const {
        auto norm_inv = 1.0 / norm, s = norm_inv * r_min;
        auto s6 = s*s*s*s*s*s, s12 = s6*s6;
        V += depth * (s12 - 2.0 * s6 + 1.0);
        dV_dn +=  depth * (s6 - s12) * norm_inv;
    }

    inline void ShiftedTruncatedLJ::asForce(VRef unit, double norm, double &V,
        Vector &dV_dr1, Vector &dV_dr2) const {
        double dV_dn = 0.0;
        kernel(norm, V, dV_dn);
        dV_dr1 -= dV_dn * unit;
        dV_dr2 += dV_dn * unit;
    }
}