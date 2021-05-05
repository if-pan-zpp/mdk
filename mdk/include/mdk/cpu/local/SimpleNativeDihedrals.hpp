#pragma once
#include <mdk/utils/Units.hpp>
#include "NativeDihedralsBase.hpp"

namespace mdk {
    class SimpleNativeDihedrals: public NativeDihedralsBase {
    public:
        double CDH = 3.33 * eps/pow(radian, 2);

        explicit SimpleNativeDihedrals(Model const& model):
            NativeDihedralsBase(model) {};

        void dihTerm(int i, double phi, double& V, double& dV_dp) const;
    };

    inline void SimpleNativeDihedrals::dihTerm(int i,
            double phi, double &V, double& dV_dp) const {
        auto diff = phi - phi0[i];
        V += 0.5 * CDH * diff * diff;
        dV_dp += CDH * diff;
    }
}