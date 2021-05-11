#pragma once
#include <mdk/cpu/data/Primitives.hpp>

namespace mdk::vl {
    struct Base {
        int i1, i2;
        Vector unit;
        double norm;
    };

    struct Free: public Base {
        enum class Status { FREE, TAKEN } status;
    };
}