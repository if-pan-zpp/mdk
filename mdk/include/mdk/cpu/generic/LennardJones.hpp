#pragma once

namespace mdk {
    class LennardJones {
    public:
        double rmin, depth;

        void perPair(VRef unit, double norm, double& V,
            Vector& dV_dr1, Vector& dV_dr2) const;
    };

    inline void LennardJones::perPair(VRef unit, double norm, double& V,
            Vector& dV_dr1, Vector& dV_dr2) const {
        auto norm_inv = 1.0 / norm, s = norm_inv * rmin;
        auto s6 = s*s*s*s*s*s, s12 = s6*s6;
        V += depth * (s12 - 2.0 * s6);

        double dV_dr = depth * (s6 - s12) * norm_inv;
        dV_dr1 -= dV_dr * unit;
        dV_dr2 += dV_dr * unit;
    }
}
