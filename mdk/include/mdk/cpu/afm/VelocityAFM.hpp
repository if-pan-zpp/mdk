#pragma once
#include <mdk/cpu/simul/State.hpp>
#include <mdk/cpu/generic/Harmonic.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>

namespace mdk {
    class VelocityAFM {
    public:
        Harmonic harm = Harmonic();

        VelocityAFM(int idx, Vector afmInitPos, Vector afmVel):
            idx(idx), afmInitPos(std::move(afmInitPos)),
            afmVel(std::move(afmVel)) {};

        inline void eval(State const& state, Dynamics& dyn) const {
            auto afmPos = afmInitPos + afmVel * state.t;
            auto diff = state.r[idx] - afmPos;
            auto diffNorm = diff.norm();

            Vector afmForce;
            harm.asForce(diff / diffNorm, diffNorm, dyn.V,
                dyn.F[idx], afmForce);
        }

    private:
        int idx;
        Vector afmInitPos, afmVel;
    };
}