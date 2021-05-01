#pragma once
#include <mdk/cpu/core/State.hpp>

namespace mdk {
    class ForceField {
    public:
        virtual void compute(State const& state, double& V, Vectors& dV_dr) = 0;
    };
}
