#pragma once
#include <cpu/generic/Harmonic.hpp>
#include <mdk/cpu/data/State.hpp>

namespace mdk {
    class HarmonicTethers {
    private:
        Scalars dist0;
        Harmonic baseV;

    public:
        double H1 = 50.0 * eps/pow(angstrom, 2);
        double H2 = 0.0;

        HarmonicTethers(Model const& model, bool fromNative);
        void eval(int i, VRef unit, double norm, double& V, Vectors& dV_dr) const;
    };

    inline void HarmonicTethers::eval(int i, VRef unit, double norm, double &V,
            Vectors &dV_dr) const {
        baseV.eval(unit, norm - dist0[i], V, dV_dr[i], dV_dr[i+1]);
    }
}
