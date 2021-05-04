#pragma once
#include <mdk/cpu/data/State.hpp>

namespace mdk {
    class NativeBondAngles {
    private:
        double CBA = 30.0 * eps/pow(radian, 2);
        Scalars theta0;

    public:
        explicit NativeBondAngles(Model const& model);
        void angleTerm(int i, double theta, double& V, double& dV_dth) const;
    };

    inline void NativeBondAngles::angleTerm(int i, double theta, double &V,
            double &dV_dth) const {
        auto diff = theta - theta0[i];
        V += CBA * diff * diff;
        dV_dth += 2.0 * CBA * diff;
    }
}
