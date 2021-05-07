#pragma once
#include <cstdint>

namespace mdk::qa {
    enum class Status: int8_t {
        FORMING, BREAKING
    };

    enum class Type: int8_t {
        BB, BS, SB, SS
    };

    struct Contact {
        Status status;
        Type type;
        double time0, r_min;
    };
}