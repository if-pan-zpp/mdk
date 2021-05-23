#pragma once
#include "Integrator.hpp"
#include "../utils/Units.hpp"
#include "../data/Masses.hpp"
#include "../simul/BoundEntity.hpp"

namespace mdk {
    class PredictorCorrector: public Integrator, BoundEntity {
    public:
        PredictorCorrector(double dt):
            dt(dt) {};

        void bind(Simulation& simulation) override;
        void integrate(State& state) override;

    private:
        double dt;
        Masses m;
        Vectors y0, y1, y2, y3, y4, y5;
    };
}