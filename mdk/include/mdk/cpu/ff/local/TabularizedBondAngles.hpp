#pragma once
#include <mdk/tools/tab/TabEnergy.hpp>
#include <cpu/data/State.hpp>

namespace mdk {
    class TabularizedBondAngles {
    private:
        struct PTValues {
            double theta_min, theta_max;
            int n;
            Scalars values;
        };
        PTValues values[9];

        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> types;

    public:
        TabularizedBondAngles(Model const& model,
            tab::TabEnergy const& tabEnergy);

        void kernel(int i,
            double theta, double& V, double& dV_dth);
    };

    inline void TabularizedBondAngles::kernel(int i,
        double theta, double &V, double &dV_dth) {

        auto& tab = values[types[i]];

        double x = (tab.n-1.0) * (theta - tab.theta_min) / (tab.theta_max - tab.theta_min);
        x = std::max(std::min(x, tab.n-1.0), 0.0);
        int x1 = floor(x), x2 = ceil(x);
        auto fx = x - x1;

        auto v1 = tab.values[x1], v2 = tab.values[x2];
        V += fx * v1 + (1.0 - fx) * v2;
        dV_dth += (v2 - v1) * (tab.n-1.0);
    }
}