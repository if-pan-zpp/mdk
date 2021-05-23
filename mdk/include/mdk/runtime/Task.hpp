#pragma once
#include <vector>
#include "Target.hpp"

namespace mdk {
    class Task {
    public:
        virtual std::vector<Target> const& req() const = 0;
        virtual std::vector<Target> const& sat() const = 0;
        virtual void run() = 0;
    };
}