#pragma once
#include "Integrator.hpp"
#include "../utils/Units.hpp"
#include "../data/Masses.hpp"
#include "../simul/SimulVar.hpp"

namespace mdk {
    /**
     * Leapfrog integration algorithm.
     * TODO: fix it, since the standalone Langevin noise has been removed.
     */
    class Leapfrog: public Integrator {
    public:
        explicit Leapfrog(double dt): dt(dt) {};

        void bind(Simulation& simulation) override;
        void init() override;
        void integrate() override;

    private:
        double dt = 0.005 * tau;
        Masses m;
        Vectors a_prev;
    };
}