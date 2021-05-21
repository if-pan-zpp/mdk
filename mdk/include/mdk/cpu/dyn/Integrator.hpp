#pragma once
#include <mdk/cpu/core/State.hpp>
#include <mdk/cpu/core/Diff.hpp>
#include <mdk/cpu/core/Simulation.hpp>

namespace mdk {
    class Integrator {
    public:
        virtual double stepTime() const = 0;
        virtual void init(Simulation& simul) = 0;
        virtual void step(Vectors& r, Vectors& v, double& t,
            Vectors const& F) = 0;
    };
}