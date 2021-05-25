#pragma once
#include "../runtime/Task.hpp"
#include "../system/State.hpp"
#include "../simul/SimulVar.hpp"
#include "../simul/Simulation.hpp"

namespace mdk {
    class Force: public Task, SimulVar {
    protected:
        State *state = nullptr;

    public:
        std::vector<Target*> req() override;
        std::vector<Target*> sat() override;
        void bind(Simulation& simulation) override;

        void run() override;
        virtual void computeForce() = 0;
    };
}