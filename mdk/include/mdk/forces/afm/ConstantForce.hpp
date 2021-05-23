#pragma once
#include "../../data/Primitives.hpp"
#include "../Force.hpp"
#include <vector>

namespace mdk {
    class ConstantForce: public Force {
    public:
        ConstantForce(int idx, Vector force):
            idx(idx), force(std::move(force)) {};

        void init(Simulation& simul) override;
        std::future<void> eval(State const& state,
            std::vector<Thread*> threads) override;

    private:
        int idx;
        Vector force;
    };
}