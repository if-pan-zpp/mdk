#pragma once
#include "../model/Model.hpp"
#include "../data/Primitives.hpp"

namespace mdk {
    class Chains {
    public:
        Model const* model;
        Integers chainIdx;
        Bytes isTerminal;
        Bytes pairs, triples, quads;
        Bytes nativePairs, nativeTriples, nativeQuads;

        Chains() = default;
        explicit Chains(Model const& model);

        inline bool sepByN(int i1, int i2, int n) const {
            return abs(i1 - i2) == n && chainIdx[i1] == chainIdx[i2] &&
                chainIdx[i1] != -1;
        }

        inline bool sepByAtLeastN(int i1, int i2, int n) const {
            return abs(i1 - i2) >= n || (chainIdx[i1] != chainIdx[i2]);
        }

        Bytes tuples(int k) const;
        Bytes nativeTuples(int k) const;
    };
}