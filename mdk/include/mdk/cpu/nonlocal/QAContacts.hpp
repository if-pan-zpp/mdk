#pragma once
#include <cstdint>
#include <variant>
#include <mdk/cpu/generic/LennardJones.hpp>
#include <mdk/cpu/generic/SinkLJ.hpp>

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
        double time0;
        std::variant<LennardJones, SinkLJ> V;
    };
}