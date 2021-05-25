#pragma once
#include "../runtime/Task.hpp"
#include "../system/State.hpp"
#include "../simul/SimulVar.hpp"
#include <chrono>

namespace mdk {
    class ProgressBar: public Task, SimulVar {
    public:
        explicit ProgressBar(double totalTime, int width = 70);

        void bind(Simulation& simulation) override;
        std::vector<Target*> req() override;
        void run() override;

    private:
        using time_point = std::chrono::high_resolution_clock::time_point;

        Simulation *simul = nullptr;
        State *state = nullptr;
        time_point realTime0;
        double totalTime;
        int width, pct = -1;
    };
}