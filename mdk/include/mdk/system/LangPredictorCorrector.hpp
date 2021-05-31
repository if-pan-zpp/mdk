#pragma once
#include "Integrator.hpp"
#include "../utils/Units.hpp"
#include "../data/Masses.hpp"

namespace mdk {
    class LangPredictorCorrector: public Integrator {
    public:
        explicit LangPredictorCorrector(double dt):
            dt(dt) {}

        void bind(Simulation& simulation) override;
        void init() override;
        void integrate() override;

        double gamma = 2.0 * f77mass / tau;

    private:
        double dt;
        Masses m;
        Vectors y0, y1, y2, y3, y4, y5;
        Random *random;
        std::vector<Random> rngs;
        Vectors gaussianNoise;
        void generateNoise();

        bool initialized = false;

        double temperature = 0.35 * eps_kB;
    };
}
