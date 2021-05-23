#pragma once

namespace mdk {
    class State;

    class Integrator {
    public:
        virtual void integrate(State& state) = 0;
    };
}