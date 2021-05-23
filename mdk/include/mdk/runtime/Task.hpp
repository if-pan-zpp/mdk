#pragma once
#include <vector>
#include "Target.hpp"

namespace mdk {
    class Task {
    public:
        virtual std::vector<Target> req() const = 0;
        virtual std::vector<Target> sat() const = 0;
        virtual void run() = 0;
    };
}