#pragma once
#include <mdk/cpu/data/Primitives.hpp>

namespace mdk {
    struct Dynamics {
        double V;
        Vectors dV_dr;
    };
}
