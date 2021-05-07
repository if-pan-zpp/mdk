#pragma once
#include <mdk/cpu/data/State.hpp>
#include <cpu/dynamics/Dynamics.hpp>
#include <mdk/cpu/data/Masses.hpp>

namespace mdk {
    class Leapfrog {
    private:
        double dt;
        Masses m;
        Vectors a_prev;

    public:
        explicit Leapfrog(Masses const& m, double dt = 0.002*nanosecond);

        void init(State& state);
        void step(State& state, Dynamics const& dyn);
    };
}