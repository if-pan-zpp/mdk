#pragma once

namespace mdk {
    class Simulation;

    class SimulVar {
    public:
        virtual void bind(Simulation& simulation) = 0;
    };
}