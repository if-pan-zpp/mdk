#pragma once
#include "../runtime/Task.hpp"
#include "../system/State.hpp"
#include "../simul/BoundEntity.hpp"
#include <chrono>

namespace mdk {
    class ProgressBar: public Task, BoundEntity {
    public:
        explicit ProgressBar(double totalTime, int width = 70);

        void bind(Simulation& simulation) override;
        std::vector<Target> req() const override;
        std::vector<Target> sat() const override;
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