#pragma once

namespace mdk {
    class Simulation;

    class BoundEntity {
    public:
        virtual void bind(Simulation& simulation) = 0;
    };
}