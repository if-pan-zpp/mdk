#pragma once
#include <mdk/cpu/data/Masses.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>
#include <memory>

namespace mdk {
    class LangevinDynamics {
    private:
        Masses m;
        std::shared_ptr<Random> random;
        double gamma;

    public:
        double T;

        explicit LangevinDynamics(Masses m, std::shared_ptr<Random> random,
            double gamma, double T): m(std::move(m)), random(std::move(random)),
            gamma(gamma), T(T) {};

        void initVel(State& state);
        void eval(State const& state, Dynamics& dyn);
    };
}