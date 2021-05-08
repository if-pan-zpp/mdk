#pragma once
#include <cpu/data/State.hpp>
#include <cpu/generic/Harmonic.hpp>
#include <cpu/dynamics/Dynamics.hpp>

namespace mdk {
    class VelocityAFM {
    private:
        Harmonic harm;
        std::vector<std::tuple<int, Vector, Vector>> afms;

    public:
        void add(int idx, Vector r0, Vector v);
        void eval(State const& state, Dynamics& dyn);
    };
}