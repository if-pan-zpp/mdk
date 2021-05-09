#pragma once
#include <mdk/cpu/data/Primitives.hpp>

namespace mdk {
    struct Dynamics {
        double V;
        Vectors F;

        explicit Dynamics(int n):
            V(0.0), F(n, Vector::Zero()) {};

        void reset();
    };
}
