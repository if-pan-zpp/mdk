#pragma once
#include <mdk/cpu/hooks/Hook.hpp>

namespace mdk {
    class ProgressBar: public Hook {
    private:
        double totalTime;
        int width, pct = -1;

    public:
        explicit ProgressBar(double totalTime, int width = 70):
            totalTime {totalTime}, width {width} {};

        void execute(Simulation& system) override;
    };
}