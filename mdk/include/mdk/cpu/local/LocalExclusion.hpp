#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/utils/Units.hpp>

namespace mdk {
    class LocalExclusion {
    public:
        double cutoff, cutoff2;
        void setCutoff(double cutoff);

        void kernel(double norm, double& V, double &dV_dn) const;
    };

    inline void LocalExclusion::kernel(double norm, double &V,
            double &dV_dn) const {
        auto scaled = cutoff / norm;
        auto s6 = pow(scaled, 6.0);

        V += eps * (s6*s6 - 2.0*s6 + 1);
        dV_dn += 12.0 * eps * (s6*s6 - s6) / norm;
    }
}