#pragma once
#include <cpu/data/State.hpp>
#include <cpu/dynamics/Dynamics.hpp>
#include <vector>

namespace mdk {
    class ConstantForce {
    private:
        std::vector<std::tuple<int, Vector>> afms;

    public:
        void add(int idx, VRef force);
        void eval(State const& state, Dynamics& dyn) const;
    };
}
