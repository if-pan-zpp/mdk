#pragma once
#include <mdk/cpu/forces/Force.hpp>
#include <mdk/utils/Random.hpp>
#include <mdk/cpu/data/Masses.hpp>
#include <memory>

namespace mdk {
    class LangevinDynamics: public Force {
    private:
        std::shared_ptr<Masses> m;
        FortranRandom random;
        double gamma, T;

    public:
        LangevinDynamics(double gamma, double T, int seed):
            random(seed), gamma(gamma), T(T) {};

        void init(Simulation& simul) override;
        std::future<void> exec(State const& state,
            std::vector<Thread*> threads) override;
    };
}