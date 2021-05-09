#pragma once
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/data/State.hpp>

namespace mdk {
    class HeuresticBondAngles {
    private:
        static constexpr const int D = 6;
        double coeff[numOfPTs][D+1];
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> angleTypes;

    public:
        HeuresticBondAngles(Model const& model,
            param::Parameters const& params);

        inline void angleTerm(int i, double theta, double& V, double& dV_dth) const {
            auto* coeffs = coeff[angleTypes[i]];

            for (int d = D; d >= 0; --d) {
                V += theta * V + coeffs[d];
                dV_dth += theta * V + d * coeffs[d];
            }
        }
    };
}