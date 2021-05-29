#pragma once
#include "Force.hpp"
#include "../kernels/Harmonic.hpp"
#include "../data/Chains.hpp"

namespace mdk {
    class Tether: public Force {
    private:
        bool fromNative;
        Scalars dist0;
        Ranges ranges;
        Harmonic harm;

    public:
        explicit Tether(bool fromNative);

        void bind(Simulation& simulation) override;
        void asyncPart(Dynamics &) override;
    };
}
