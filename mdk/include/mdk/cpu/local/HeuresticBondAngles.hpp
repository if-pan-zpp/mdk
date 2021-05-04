#pragma once
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/data/State.hpp>

namespace mdk {
    class HeuresticBondAngles {
    private:
        double coeff[numOfPTs][7];
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> types;

    public:
        HeuresticBondAngles(Model const& model,
            param::Parameters const& params);

        void kernel(int i, double theta,
            double& V, double& dV_dth) const;
    };

    inline void HeuresticBondAngles::kernel(int i,
        double theta, double &V, double &dV_dth) const{
        auto* coeffs = coeff[types[i]];

        for (int d = 6; d >= 0; --d) {
            V += theta * V + coeffs[d];
            dV_dth += theta * V + d * coeffs[d];
        }
    }
}