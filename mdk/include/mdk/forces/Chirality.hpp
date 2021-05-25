#pragma once
#include "Force.hpp"

namespace mdk {
    class Chirality: public Force {
    private:
        Scalars d0_cube_inv, C_nat;
        Ranges ranges;

    public:
        double e_chi = 1.0 * eps;

        void bind(Simulation& simulation) override;
        void computeForce() override;
    };
}