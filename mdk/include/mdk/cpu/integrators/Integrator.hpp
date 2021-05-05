#pragma once
#include <mdk/cpu/data/Primitives.hpp>

namespace mdk {
    class Integrator {
    public:
        virtual void step(Vectors& r, Vectors& v, Vectors& dV_dr) = 0;
    };
}
