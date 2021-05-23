#pragma once
#include "Integrator.hpp"
#include "../utils/Units.hpp"
#include "../data/Masses.hpp"
#include "../simul/BoundEntity.hpp"

namespace mdk {
    class Leapfrog: public Integrator, BoundEntity {
    public:
        explicit Leapfrog(double dt): dt(dt) {};

        void bind(Simulation& simulation) override;
        void integrate(State& state) override;

    private:
        double dt = 0.005 * tau;
        Masses m;
        Vectors a_prev;
    };
}