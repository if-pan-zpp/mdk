#pragma once
#include <mdk/cpu/core/State.hpp>

namespace mdk {
    class HarmonicTethers {
    private:
        Scalars dist0;

        double H1 = 50.0 * eps/pow(angstrom, 2);
        double H2 = 0.0;

    public:
        HarmonicTethers(Model const& model, bool fromNative);
        void kernel(int i1, int i2, double r, double& V, double& dV_dl);
    };

    inline void HarmonicTethers::kernel(int i1, int i2, double r, double& V, double &dV_dl) {
        auto diff = r - dist0[i1];
        auto diff2 = diff * diff;
        V = diff * diff * (H1 + H2 * diff2);
        dV_dl = diff * (2.0 * H1 + 4.0 * H2 * diff2);
    }
}
