#pragma once
#include <mdk/cpu/forces/angle/BondAngleBase.hpp>

namespace mdk {
    class NativeBondAngle: public BondAngleBase<NativeBondAngle> {
    private:
        Scalars theta0;

    public:
        double CBA = 30.0 * eps/pow(rad, 2);
        explicit NativeBondAngle(Model const& model);

        void bondAngleTerm(int i, double theta, double& V, double& dV_dth) const;
    };
}