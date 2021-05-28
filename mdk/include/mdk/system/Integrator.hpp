#pragma once
#include "../simul/SimulVar.hpp"
#include "../simul/Simulation.hpp"
#include "State.hpp"

namespace mdk {
    class Integrator: public SimulVar {
    protected:
        State *state = nullptr;

    public:
        void bind(Simulation& simulation) override;
        virtual void init() = 0;
        virtual void integrate() = 0;
    };
}