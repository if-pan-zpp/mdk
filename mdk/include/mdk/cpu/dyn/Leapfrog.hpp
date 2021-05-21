#pragma once
#include <mdk/cpu/dyn/Integrator.hpp>
#include <mdk/cpu/data/Masses.hpp>

namespace mdk {
    class Leapfrog: public Integrator {
    private:
        std::shared_ptr<Masses> m;
        Vectors a_prev;

    public:
        double dt;
        explicit Leapfrog(double dt = 0.002*nanosecond);

        double stepTime() const override;
        void init(Simulation& simul) override;
        void step(Vectors& r, Vectors& v, double& t,
            Vectors const& F) override;
    };
}