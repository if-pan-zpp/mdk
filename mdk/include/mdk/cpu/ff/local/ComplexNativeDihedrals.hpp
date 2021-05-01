#pragma once
#include <mdk/utils/Units.hpp>

namespace mdk {
    class ComplexNativeDihedrals {
    private:
        double CDA = 0.66 * eps / pow(radian, 2.0);
        double CDB = 0.66 * eps / pow(radian, 2.0);

    public:
        void kernel(int i1, int i2, int i3, int i4,
            double phi, double phi0,
            double& V, double& dV_dp);
    };

    inline void ComplexNativeDihedrals::kernel(int i1, int i2, int i3, int i4,
            double phi, double phi0, double &V, double &dV_dp) {
        V = CDA * (1.0 - cos(phi - phi0)) + CDB * (1.0 - cos(3.0 * (phi - phi0)));
        dV_dp = CDA * sin(phi - phi0) + 3.0 * CDB * sin(3.0 * (phi - phi0));
    }
}