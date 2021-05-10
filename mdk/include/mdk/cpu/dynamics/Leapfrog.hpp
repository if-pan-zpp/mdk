#pragma once
#include <mdk/cpu/simul/State.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>
#include <mdk/cpu/data/Masses.hpp>

namespace mdk {
    class Leapfrog {
    private:
        Masses m;
        Vectors a_prev;

    public:
        double dt;

        explicit Leapfrog(Masses m, double dt = 0.002*nanosecond);

        void step(State& state, Dynamics const& dyn);
    };
}