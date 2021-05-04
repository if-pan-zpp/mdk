#pragma once
#include <mdk/utils/Units.hpp>
#include <mdk/cpu/data/Primitives.hpp>

namespace mdk {
    class Harmonic {
    public:
        double H1 = 50.0 * eps/pow(angstrom, 2.0);
        double H2 = 0.0;

        void eval(VRef unit, double diff, double& V,
            Vector& dV_dr1, Vector& dV_dr2) const;
    };

    inline void Harmonic::eval(VRef unit, double diff, double &V,
            Vector &dV_dr1, Vector &dV_dr2) const {
        auto diff2 = diff*diff;
        V += diff2 * (H1 + H2 * diff2);

        auto dV_doff = diff * (2.0 * H1 + 4.0 * H2 * diff2);
        dV_dr1 -= dV_doff * unit;
        dV_dr2 += dV_doff * unit;
    }
}
