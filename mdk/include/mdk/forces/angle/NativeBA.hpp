#pragma once
#include "../../data/Primitives.hpp"
#include "../../simul/SimulVar.hpp"
#include "../../utils/Units.hpp"

namespace mdk {
    class NativeBA: public SimulVar {
    private:
        friend class BondAngles;
        Bytes isNative;
        Scalars theta0;
        double CBA = 30.0 * eps/pow(rad, 2);

    public:
        void bind(Simulation& simulation) override;

        void term(int i, double theta, double& V, double& dV_dth) const {
            auto diff = theta - theta0[i];
            V += CBA * diff * diff;
            dV_dth += 2.0 * CBA * diff;
        }
    };
}