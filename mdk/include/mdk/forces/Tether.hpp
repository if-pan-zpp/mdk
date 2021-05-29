#pragma once
#include "Force.hpp"
#include "../kernels/Harmonic.hpp"
#include "../data/Chains.hpp"

namespace mdk {
    class Tether: public Force {
    public:
        explicit Tether(bool fromNative);

        void bind(Simulation& simulation) override;
        void asyncPart(Dynamics &) override;
    private:
        Harmonic harm;
        bool fromNative;

        int n;
        Scalars dist0;
        Bytes isConnected;
    };
}
