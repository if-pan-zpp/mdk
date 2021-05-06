#pragma once
#include <mdk/utils/Units.hpp>
#include <mdk/cpu/data/Primitives.hpp>

namespace mdk {
    class Harmonic {
    public:
        double H1 = 50.0 * eps/pow(angstrom, 2.0);
        double H2 = 0.0;

        void kernel(double diff, double& V, double& dV_dn) const;
        void asForce(VRef unit, double diff, double& V,
            Vector& dV_dr1, Vector& dV_dr2) const;
    };

    inline void Harmonic::kernel(double diff, double& V, double& dV_dn) const {
        auto diff2 = diff*diff;
        V += diff2 * (H1 + H2 * diff);
        dV_dn += diff * (2.0 * H1 + 4.0 * H2 * diff2);
    }

    inline void Harmonic::asForce(VRef unit, double diff, double &V,
            Vector &dV_dr1, Vector &dV_dr2) const {
        double dV_dn = 0.0;
        kernel(diff, V, dV_dn);
        dV_dr1 -= dV_dn * unit;
        dV_dr2 += dV_dn * unit;
    }
}
