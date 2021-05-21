#pragma once
#include <mdk/cpu/core/State.hpp>
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/cpu/forces/qa/QADiffEntry.hpp>

namespace mdk {
    class Diff {
    public:
        double V = 0.0;
        Vectors F;
        std::vector<QADiffEntry> qaDiffs;

        void init(State const& state);
        void reset();
        static Diff& reduce(std::vector<Diff>& diffs);
    };
}