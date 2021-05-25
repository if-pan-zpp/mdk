#pragma once
#include "../NonlocalForce.hpp"

namespace mdk {
    class ESBase: public NonlocalForce {
    protected:
        struct Contact {
            int i1, i2;
            double q1_x_q2;
        };
        std::vector<Contact> pairs;
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> charge;

        void vlUpdateHook() override;

    public:
        void bind(Simulation& simulation) override;
    };
}
