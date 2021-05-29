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
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> angleTypes;

    public:
        void bind(Simulation& simulation) override;

        void term(int i, double theta, double& V, double& dV_dth) const {
            auto coeffs = coeff[angleTypes[i]];
            for (int d = D; d >= 0; --d) {
                V += theta * V + coeffs[d];
                dV_dth += theta * V + d * coeffs[d];
            }
        }
    };
}