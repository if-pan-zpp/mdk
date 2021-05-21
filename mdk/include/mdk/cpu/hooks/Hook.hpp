#pragma once

namespace mdk {
    class Simulation;

    class Hook {
    public:
        virtual void init(Simulation& simul) = 0;
        virtual void exec(Simulation const& simul) = 0;
    };
}
