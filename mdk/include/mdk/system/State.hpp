#pragma once
#include "../data/Primitives.hpp"
#include "../runtime/TaskFactory.hpp"
#include "../utils/Topology.hpp"
#include "../model/Model.hpp"
#include "Integrator.hpp"

namespace mdk {
    class Dynamics {
    public:
        double V = 0.0;
        Vectors F;
    };

    class State: public TaskFactory {
    public:
        int n;
        Vectors r, v;
        double t;
        Topology top;

        Dynamics dyn;

        Target dynReset = Target::create();
        Target canUpdate = Target::create();
        Target stateUpdated = Target::create();
        std::vector<std::unique_ptr<Task>> tasks() override;

        void updateModel(Model& model);

    private:
        std::unique_ptr<Integrator> integrator;
    };
}