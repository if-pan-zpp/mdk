#pragma once
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/core/State.hpp>

namespace mdk {
    class HeuresticBondAngles {
    private:
        double coeff[9][7];
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> types;

    public:
        HeuresticBondAngles(Model const& model,
            param::Parameters const& params);

        void kernel(int i1, int i2, int i3, int8_t type, double theta,
            double& V, double& dV_dth);
    };

    inline void HeuresticBondAngles::kernel(int i1, int i2, int i3, int8_t type,
        double theta, double &V, double &dV_dth) {
        auto* coeffs = coeff[types[i2]];

        V = dV_dth = 0;
        for (int d = 6; d >= 0; --d) {
            V += theta * V + coeffs[d];
            dV_dth += theta * V + d * coeffs[d];
        }
    }
}