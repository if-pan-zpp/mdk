#pragma once
#include <vector>
#include <cstdint>

namespace mdk {
    struct QAContact {
        int i1, i2;
        enum class Status: int8_t { FORMING, BREAKING, REMOVED } status;
        enum class Type: int8_t { BB, BS, SB, SS } type;
        double t0;
    };

    struct QAFreePair {
        int i1, i2;
        enum class Status: int8_t { FREE, TAKEN } status;
    };

    class QAState {
    public:
        std::vector<QAContact> pairs;
        std::vector<QAFreePair> freePairs;
    };
}