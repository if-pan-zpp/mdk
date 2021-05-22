#pragma once
#include <mdk/cpu/kernels/Harmonic.hpp>
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/cpu/forces/Force.hpp>

namespace mdk {
    class VelocityAFM: public Force {
    public:
        VelocityAFM(int idx, Vector afmInitPos, Vector afmVel):
            idx(idx), afmInitPos(std::move(afmInitPos)),
            afmVel(std::move(afmVel)) {};

        void init(Simulation& simul) override;
        std::future<void> eval(State const& state,
            std::vector<Thread*> threads) override;

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
        Harmonic harm = Harmonic();
    };
}