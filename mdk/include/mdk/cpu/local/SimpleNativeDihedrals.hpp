#pragma once
#include <mdk/utils/Units.hpp>

namespace mdk {
    class SimpleNativeDihedrals {
    private:
        double CDH = 3.33 * eps/pow(radian, 2);

    public:
        void kernel(
            double phi, double phi0,
            double& V, double& dV_dp) const;
    };

    inline void SimpleNativeDihedrals::kernel(
        double phi, double phi0, double &V, double& dV_dp) const {
        auto diff = phi - phi0;
        V += 0.5 * CDH * diff * diff;
        dV_dp += CDH * diff;
    }
}