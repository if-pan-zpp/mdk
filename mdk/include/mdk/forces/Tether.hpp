#pragma once
#include <forces/Force.hpp>
#include <mdk/tools/model/Model.hpp>
#include <kernels/Harmonic.hpp>
#include <data/Chains.hpp>

namespace mdk {
    class Tether: public Force {
    private:
        bool fromNative;
        Scalars dist0;
        Ranges ranges;
        Harmonic harm;

    public:
        explicit Tether(bool fromNative);

        void init(Simulation& simul) override;
        std::future<void> eval(State const& state,
            std::vector<Thread*> threads) override;
    };
}