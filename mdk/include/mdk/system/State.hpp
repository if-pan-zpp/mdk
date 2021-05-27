#pragma once
#include "../data/Primitives.hpp"
#include "../utils/Topology.hpp"
#include "../model/Model.hpp"
#include "../simul/SimulVar.hpp"

namespace mdk {
    class Dynamics {
    public:
        double V = 0.0;
        Vectors F;
    };

    class State: public SimulVar {
    public:
        int n;
        Vectors r, v;
        double t;
        Topology top;

        Dynamics dyn;

        void prepareDyn();
        void bind(Simulation& simul) override;
        void exportTo(Model& model);
    };
}