#pragma once
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class Sequences {
    public:
        Bytes isNative, isConnected, isTerminal;
        Integers chainIdx;
        std::vector<std::pair<int, int>> chainBounds;

        Sequences() = default;
        explicit Sequences(Model const& model);

        [[nodiscard]] inline bool sepByN(int i1, int i2, int n) const {
            return abs(i1 - i2) == n && chainIdx[i1] == chainIdx[i2] &&
                chainIdx[i1] != -1;
        }
    };
}