#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/cpu/data/State.hpp>
#include <cpu/nonlocal/qa/Diffs.hpp>

namespace mdk {
    class StateDiff {
    public:
        double V;
        Vectors dV_dr;
        Vectors n, h;
        qa::Diffs qaDiffs;
    };
}