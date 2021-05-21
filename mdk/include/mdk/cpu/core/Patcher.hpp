#pragma once
#include <mdk/cpu/core/Diff.hpp>
#include <mdk/cpu/core/State.hpp>
#include <mdk/cpu/dyn/Integrator.hpp>
#include <mdk/cpu/forces/qa/QuasiAdiabatic.hpp>
#include <mdk/cpu/core/Simulation.hpp>

namespace mdk {
    class Patcher {
    public:
        std::unique_ptr<Integrator> integrator;
        QuasiAdiabatic *qa;

        void patch(State& state, Diff& diff);
    };
}