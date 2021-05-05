#pragma once
#include <cstdint>

namespace mdk::qa {
    enum class ContactType: int8_t {
        NONE, BB, BS, SB, SS
    };
}