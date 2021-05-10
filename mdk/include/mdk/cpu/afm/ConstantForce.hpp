#pragma once
#include <mdk/cpu/simul/State.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>
#include <vector>

namespace mdk {
    class ConstantForce {
    public:
        ConstantForce(int idx, Vector force):
            idx(idx), force(std::move(force)) {};

        inline void eval(Dynamics& dyn) const {
            dyn.F[idx] += force;
        }

    private:
        int idx;
        Vector force;
    };
}
