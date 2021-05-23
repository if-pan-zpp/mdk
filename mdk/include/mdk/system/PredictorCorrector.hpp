#pragma once
#include "Integrator.hpp"
#include "../utils/Units.hpp"
#include "../data/Masses.hpp"

namespace mdk {
    class PredictorCorrector: public Integrator {
    public:
        PredictorCorrector(double dt, Masses m,
            Model const& model);

        void integrate(State& state) override;

    private:
        double dt;
        Masses m;
        Vectors y0, y1, y2, y3, y4, y5;
    };
}