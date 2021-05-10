#pragma once
#include <mdk/cpu/simul/State.hpp>

namespace mdk {
    class NativeBondAngles {
    private:
        double CBA = 30.0 * eps/pow(rad, 2);
        Scalars theta0;

    public:
        explicit NativeBondAngles(Model const& model);

        inline void angleTerm(int i, double theta, double& V, double& dV_dth) const {
            auto diff = theta - theta0[i];
            V += CBA * diff * diff;
            dV_dth += 2.0 * CBA * diff;
        }
    };
}
