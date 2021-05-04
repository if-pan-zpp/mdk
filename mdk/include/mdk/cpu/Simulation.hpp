#pragma once
#include <mdk/cpu/local/LocalPass.hpp>
#include <mdk/cpu/nonlocal/NonlocalPass.hpp>
#include <mdk/cpu/integrators/Integrator.hpp>

namespace mdk {
    class Simulation {
    private:
        State state;

        LocalPass localPass;
        NonlocalPass nonlocalPass;
        std::unique_ptr<Integrator> integrator;

        bool running = true;

    public:
        void run();
    };
}