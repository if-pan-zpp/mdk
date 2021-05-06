#pragma once
#include <vector>
#include <mdk/cpu/nonlocal/NormalVL.hpp>
#include <cpu/nonlocal/qa/Stats.hpp>

namespace mdk::qa {
    struct Diff {
        vl::Pair<NormalData> pair;
        Stat stats[2];
    };

    using Diffs = std::vector<Diff>;
}