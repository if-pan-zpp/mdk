#pragma once
#include <mdk/cpu/data/State.hpp>

namespace mdk {
    class NativeBondAngles {
    private:
        double CBA = 30.0 * eps/pow(radian, 2);

    public:
        void kernel(
            double theta, double theta0,
            double& V, double& dV_dth) const;
    };

    inline void NativeBondAngles::kernel(double theta,
        double theta0, double &V, double &dV_dth) const {
        auto diff = theta - theta0;
        V += CBA * diff * diff;
        dV_dth += 2.0 * CBA * diff;
    }
}
