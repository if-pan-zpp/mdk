#pragma once
#include "Force.hpp"
#include "../utils/Random.hpp"
#include "../data/Masses.hpp"
#include <memory>

namespace mdk {
    class LangevinDynamics: public Force {
    private:
        Masses const *m = nullptr;
        Random *random = nullptr;
        double gamma, T;

    public:
        LangevinDynamics(double gamma, double T):
            gamma(gamma), T(T) {};

        void bind(Simulation& simulation) override;
        void asyncPart() override;
    };
}