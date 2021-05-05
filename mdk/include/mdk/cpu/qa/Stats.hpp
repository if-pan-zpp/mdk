#pragma once
#include <cstdint>
#include <vector>

namespace mdk::qa {
    struct Stat {
        int8_t backbone = 0;
        int8_t sidechain = 0;
        int8_t hydrophobicSS = 0;
        int8_t polarSS = 0;
    };

    using Stats = std::vector<Stat>;
}
