#pragma once
#include <mdk/cpu/dynamics/Dynamics.hpp>
#include <mdk/cpu/kernels/ShiftedTruncatedLJ.hpp>
#include <mdk/cpu/verlet/Base.hpp>

namespace mdk {
    class PauliExclusion: public ShiftedTruncatedLJ {
    public:
        inline void perPair(vl::Base const& p, Dynamics& dyn) {
            asForce(p.unit, p.norm, dyn.V, dyn.F[p.i1], dyn.F[p.i2]);
        }
    };
}