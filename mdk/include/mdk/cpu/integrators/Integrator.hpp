#pragma once
#include <mdk/cpu/data/Primitives.hpp>

namespace mdk {
    class Integrator {
    public:
        virtual void step(Vectors const& dV_dr, double& t, Vectors& r,
            Vectors& v) = 0;
    };
}
