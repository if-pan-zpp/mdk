#pragma once
#include "../system/State.hpp"
#include "../simul/SimulVar.hpp"
#include "Hook.hpp"
#include <chrono>

namespace mdk {
    class ProgressBar: public Hook, SimulVar {
    public:
        explicit ProgressBar(double totalTime, int width = 70);

        void bind(Simulation& simulation) override;
        void execute() override;

    private:
        using time_point = std::chrono::high_resolution_clock::time_point;

        Simulation *simul = nullptr;
        State *state = nullptr;
        time_point realTime0;
        double totalTime;
        int width, pct = -1;
    };
}