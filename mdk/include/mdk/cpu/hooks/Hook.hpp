#pragma once

namespace mdk {
    class Simulation;

    class Hook {
    public:
        virtual void execute(Simulation& system) = 0;
    };
}
