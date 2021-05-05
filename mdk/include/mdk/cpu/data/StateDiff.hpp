#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/cpu/data/State.hpp>
#include <cpu/qa/Diffs.hpp>

namespace mdk {
    class StateDiff {
    public:
        double V;
        Vectors dV_dr;
        qa::Diffs qaDiffs;
    };
}