#pragma once
#include "Integrator.hpp"
#include "../utils/Units.hpp"
#include "../data/Masses.hpp"

namespace mdk {
    class Leapfrog: public Integrator {
    public:
        Leapfrog(double dt, Masses m):
            dt(dt), m(std::move(m)), a_prev(m.size()) {};

        void integrate(State& state) override;

    private:
        double dt = 0.005 * tau;
        Masses m;
        Vectors a_prev;
    };
}