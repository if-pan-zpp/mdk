#pragma once
#include <mdk/cpu/generic/Harmonic.hpp>
#include <mdk/cpu/simul/State.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>

namespace mdk {
    class HarmonicTethers {
    private:
        Scalars dist0;
        Harmonic harm;

    public:
        HarmonicTethers(Model const& model, bool fromNative);

        inline void perPair(int i, VRef unit, double norm, double& V,
            Vector& F1, Vector& F2) const {
            harm.asForce(unit, norm - dist0[i], V, F1, F2);
        }
    };
}
