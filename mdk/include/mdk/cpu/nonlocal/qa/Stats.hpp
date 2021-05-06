#pragma once
#include <cstdint>
#include <vector>

namespace mdk::qa {
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

    using Stats = std::vector<Stat>;
}
