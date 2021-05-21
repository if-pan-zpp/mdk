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

    struct Stat {
        int8_t backbone = 0;
        int8_t sidechain = 0;
        int8_t hydrophobicSS = 0;
        int8_t polarSS = 0;

        Stat& operator+=(Stat const& other);
        Stat operator+(Stat const& other) const;
        Stat operator-() const;
        Stat& operator-=(Stat const& other);
        Stat operator-(Stat const& other) const;
    };

    struct QAFreePair {
        int i1, i2;
        enum class Status: int8_t { FREE, TAKEN } status;
    };

    class QAState {
    public:
        std::vector<Stat> stats;
        std::vector<QAContact> pairs;
        std::vector<QAFreePair> freePairs;
    };
}