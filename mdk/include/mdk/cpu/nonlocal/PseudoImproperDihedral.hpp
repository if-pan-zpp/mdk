#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/cpu/data/StateDiff.hpp>

namespace mdk {
    class PseudoImproperDihedral {
    public:
        void kernel() const;
    };
}