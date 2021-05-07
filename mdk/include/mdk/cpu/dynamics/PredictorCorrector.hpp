#pragma once
#include <mdk/cpu/data/State.hpp>
#include <cpu/dynamics/Dynamics.hpp>
#include <mdk/cpu/data/Masses.hpp>

namespace mdk {
    class PredictorCorrector {
    private:
        Vectors y0, y1, y2, y3, y4, y5;
        Masses m;

    public:
        double dt;
        PredictorCorrector() = default;
        explicit PredictorCorrector(Masses const& m, double dt = 0.002*nanosecond);

        void init(State& state);
        void predict(State& state);
        void correct(State& state, Dynamics const& dyn);
    };
}