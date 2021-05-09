#pragma once
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class Sequences {
    public:
        Bytes isNative, isConnected, isTerminal;
        Integers chainIdx;

        Sequences() = default;
        explicit Sequences(Model const& model);

        inline bool sepByN(int i1, int i2, int n) const {
            return abs(i1 - i2) == n && chainIdx[i1] == chainIdx[i2] &&
                chainIdx[i1] != -1;
        }
    };
}