#pragma once
#include "../simul/SimulVar.hpp"
#include "../runtime/Task.hpp"
#include "simul/Simulation.hpp"
#include "State.hpp"

namespace mdk {
    class Integrator: public SimulVar, Task {
    protected:
        State *state;

    public:
        std::vector<Target*> req() override;
        std::vector<Target*> sat() override;

        void bind(Simulation& simulation) override;
        void run() override;

        virtual void integrate() = 0;
    };
}