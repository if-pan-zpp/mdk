#pragma once
#include <mdk/cpu/data/State.hpp>
#include <mdk/cpu/data/Dynamics.hpp>

namespace mdk {
    class Integrator {
    public:
        virtual void step(State& state, Dynamics const& dyn) = 0;
    };
}
