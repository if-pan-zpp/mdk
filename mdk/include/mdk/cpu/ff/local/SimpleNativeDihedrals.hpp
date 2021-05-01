#pragma once
#include <utils/Units.hpp>

namespace mdk {
    class SimpleNativeDihedrals {
    private:
        double CDH = 3.33 * eps/pow(radian, 2);

    public:
        void kernel(int i1, int i2, int i3, int i4,
            double phi, double phi0,
            double& V, double& dV_dp);
    };

    inline void SimpleNativeDihedrals::kernel(int i1, int i2, int i3, int i4,
        double phi, double phi0, double &V, double& dV_dp) {
        auto diff = phi - phi0;
        V = 0.5 * CDH * diff * diff;
        dV_dp = CDH * diff;
    }
}