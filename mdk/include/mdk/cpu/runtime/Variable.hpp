#pragma once
#include <mdk/cpu/runtime/Task.hpp>
#include <mdk/cpu/runtime/Target.hpp>

namespace mdk {
    class Variable {
    public:
        virtual Target modified() const = 0;
        virtual Target updated() const = 0;
        virtual Task modInit() const = 0;
        virtual Task update() const = 0;
    };
}