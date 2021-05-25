#pragma once
#include <vector>
#include "Target.hpp"

namespace mdk {
    class Task {
    public:
        virtual std::vector<Target*> req();
        virtual std::vector<Target*> sat();
        virtual void run() = 0;
    };
}