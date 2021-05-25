#pragma once
#include "../data/Primitives.hpp"
#include "../runtime/TaskFactory.hpp"
#include "../utils/Topology.hpp"
#include "../model/Model.hpp"
#include "../simul/SimulVar.hpp"
#include "Integrator.hpp"

namespace mdk {
    class Dynamics {
    public:
        double V = 0.0;
        Vectors F;
    };

    class State: public TaskFactory, SimulVar {
    public:
        int n;
        Vectors r, v;
        double t;
        Topology top;

        Dynamics dyn;

        Target forcesReset, beforeUpdate, stateUpdated;
        std::vector<std::unique_ptr<Task>> tasks() override;

        void bind(Simulation& simul) override;
        void exportTo(Model& model);
    };
}