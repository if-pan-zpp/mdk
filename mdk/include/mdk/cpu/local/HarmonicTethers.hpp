#pragma once
#include <mdk/cpu/data/State.hpp>

namespace mdk {
    class HarmonicTethers {
    private:
        Scalars dist0;

    public:
        double H1 = 50.0 * eps/pow(angstrom, 2);
        double H2 = 0.0;

        HarmonicTethers(Model const& model, bool fromNative);
        void kernel(int i, double norm, double& V, double& dV_dn) const;
    };

    inline void HarmonicTethers::kernel(int i, double norm, double& V, double &dV_dn) const {
        auto diff = norm - dist0[i];
        V += H1 * pow(diff, 2.0) + H2 * pow(diff, 4.0);
        dV_dn += 2.0 * H1 * diff + 4.0 * H2 * pow(diff, 3.0);
    }
}
