#pragma once
#include "BondAngleBase.hpp"

namespace mdk {
    class NativeBondAngle: public BondAngleBase<NativeBondAngle> {
    private:
        Scalars theta0;
        double CBA = 30.0 * eps/pow(rad, 2);

    public:
        void bind(Simulation& simulation) override;
        void bondAngleTerm(int i, double theta, double& V, double& dV_dth) const;
    };
}