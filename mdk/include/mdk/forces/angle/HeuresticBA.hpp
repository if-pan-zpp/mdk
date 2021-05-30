#pragma once
#include "../../data/Primitives.hpp"
#include "../../simul/SimulVar.hpp"
#include "../../utils/PairType.hpp"

namespace mdk {
    class HeuresticBA: public SimulVar {
    private:
        friend class BondAngles;
        static constexpr const int D = 6;
        double coeff[numOfPTs][D+1];
        Bytes angleTypes;

    public:
        void bind(Simulation& simulation) override;

        void term(int i, double theta, double& V, double& dV_dth) const {
            double const* coeffs = coeff[angleTypes[i]];
            double V_loc = 0.0;
            double dV_dth_loc = 0.0;
            for (int d = D; d >= 0; --d) {
                if (d > 0) dV_dth_loc = d * coeffs[d] + theta * dV_dth_loc;
                V_loc = coeffs[d] + theta * V_loc;
            }

            V += V_loc;
            dV_dth += dV_dth_loc;
        }
    };
}
