#pragma once
#include <mdk/cpu/data/Masses.hpp>
#include <cpu/dynamics/Dynamics.hpp>

namespace mdk {
    class LangevinDynamics {
    private:
        Masses m;
        double gamma;
        double T;

    public:
        explicit LangevinDynamics(Masses const& m, double gamma);

        void init(Random &random, State& state);
        void eval(Random& random, State const& state, Dynamics& dyn);
    };
}