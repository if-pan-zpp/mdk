#pragma once
#include <mdk/utils/Units.hpp>
#include <mdk/cpu/local/NativeDihedralsBase.hpp>

namespace mdk {
    class SimpleNativeDihedrals: public NativeDihedralsBase {
    public:
        double CDH = 3.33 * eps/pow(rad, 2);

        explicit SimpleNativeDihedrals(Model const& model):
            NativeDihedralsBase(model) {};

        inline void dihTerm(int i, double phi, double& V, double& dV_dp) const {
            auto diff = phi - phi0[i];
            V += 0.5 * CDH * diff * diff;
            dV_dp += CDH * diff;
        }
    };
}