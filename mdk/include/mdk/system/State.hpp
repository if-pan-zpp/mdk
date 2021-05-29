#pragma once
#include "../data/Primitives.hpp"
#include "../utils/Topology.hpp"
#include "../model/Model.hpp"
#include "../simul/SimulVar.hpp"

namespace mdk {
    struct Dynamics {
        double V = 0.0;
        Vectors F;

        void zero(int n) {
            V = 0.0;
            F = Vectors(n, Vector::Zero());
        }
    };

    class State: public SimulVar {
    public:
        int n;
        Vectors r, v;
        double t;
        Topology top;

        Dynamics dyn;

        void prepareDyn();
        void updateWithDyn(Dynamics const& othDyn);
        void bind(Simulation& simul) override;
        void exportTo(Model& model);
    };
}
