#pragma once
#include <vector>
#include <future>
#include <mdk/cpu/runtime/Target.hpp>

namespace mdk {
    class Task {
    public:
        virtual std::vector<Target> prerequisites() const = 0;
        virtual std::vector<Target> satisfied() const = 0;
        virtual std::future<void> perform() = 0;
    };
}