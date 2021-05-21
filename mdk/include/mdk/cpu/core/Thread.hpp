#pragma once
#include <vector>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <thread>
#include <mdk/cpu/core/Simulation.hpp>
#include <mdk/cpu/core/Diff.hpp>

namespace mdk {
    class Thread {
    private:
        friend class Simulation;
        Simulation const& simul;

        explicit Thread(Simulation const& simul):
            simul(simul) {};

    public:
        std::thread thr;
        Diff diff;
    };
}
