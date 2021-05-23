#pragma once
#include "../model/Model.hpp"
#include "../data/Primitives.hpp"

namespace mdk {
    class Chains {
    public:
        Bytes isNative, isConnected, isTerminal;
        Integers chainIdx;
        Ranges chainBounds;

        Chains() = default;
        explicit Chains(Model const& model);

        [[nodiscard]] inline bool sepByN(int i1, int i2, int n) const {
            return abs(i1 - i2) == n && chainIdx[i1] == chainIdx[i2] &&
                chainIdx[i1] != -1;
        }
    };
}