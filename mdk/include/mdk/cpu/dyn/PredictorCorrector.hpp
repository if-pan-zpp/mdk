#pragma once
#include <mdk/cpu/dyn/Integrator.hpp>
#include <mdk/cpu/data/Masses.hpp>

namespace mdk {
    class PredictorCorrector: public Integrator {
    private:
        Vectors y0, y1, y2, y3, y4, y5;
        std::shared_ptr<Masses> m;

    public:
        double dt;
        explicit PredictorCorrector(double dt = 0.005*nanosecond);

        double stepTime() const override;
        void init(Simulation& simul) override;
        void step(Vectors& r, Vectors& v, double& t,
            Vectors const& F) override;
    };
}