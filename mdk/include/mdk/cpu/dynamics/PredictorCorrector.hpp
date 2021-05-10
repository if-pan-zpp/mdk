#pragma once
#include <mdk/cpu/simul/State.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>
#include <mdk/cpu/data/Masses.hpp>

namespace mdk {
    class PredictorCorrector {
    private:
        Vectors y0, y1, y2, y3, y4, y5;
        Masses m;

    public:
        double dt;
        explicit PredictorCorrector(Masses m, double dt = 0.005*nanosecond);

        void init(State& state);
        void predict(State& state);
        void correct(State& state, Dynamics const& dyn);
    };
}