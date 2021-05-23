#pragma once
#include <future>
#include <mdk/cpu/core/Thread.hpp>

namespace mdk {
    class Simulation;

    class Force {
    public:
        virtual void init(Simulation& simul) = 0;
        virtual std::future<void> eval(State const& state,
            std::vector<Thread*> threads) = 0;
    };
}