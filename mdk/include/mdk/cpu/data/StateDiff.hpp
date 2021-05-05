#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/cpu/data/State.hpp>

namespace mdk {
    class StateDiff {
    public:
        double V;
        Vectors dV_dr;
    };
}