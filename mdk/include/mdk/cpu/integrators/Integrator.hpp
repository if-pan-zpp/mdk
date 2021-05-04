#pragma once
#include <mdk/cpu/data/StateDiff.hpp>

namespace mdk {
    class Integrator {
    public:
        virtual void step(State& state, StateDiff const& sd) = 0;
    };
}
