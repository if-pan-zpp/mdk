#pragma once
#include "Hook.hpp"
#include <chrono>

namespace mdk {
    class ProgressBar: public Hook {
    private:
        using time_point = std::chrono::high_resolution_clock::time_point;
        time_point realTime0;
        double totalTime;
        int width, pct = -1;

    public:
        explicit ProgressBar(double totalTime, int width = 70);

        void execute(Simulation& system) override;
    };
}