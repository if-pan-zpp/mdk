#pragma once
#include <mdk/cpu/kernels/Harmonic.hpp>
#include <mdk/cpu/simul/State.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>

namespace mdk {
    class HarmonicTethers {
    private:
        Scalars dist0;
        Harmonic harm;

    public:
        HarmonicTethers(Model const& model, bool fromNative);

        template<typename T1, typename T2>
        inline void perPair(int i, VRef unit, double norm, double& V,
            T1 F1, T2 F2) const {

            harm.asForce(unit, norm - dist0[i], V, F1, F2);
        }
    };
}
