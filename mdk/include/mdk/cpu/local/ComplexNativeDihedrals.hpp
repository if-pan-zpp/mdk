#pragma once
#include <mdk/utils/Units.hpp>
#include <mdk/cpu/local/NativeDihedralsBase.hpp>

namespace mdk {
    class ComplexNativeDihedrals: public NativeDihedralsBase {
    public:
        double CDA = 0.66 * eps / pow(rad, 2.0);
        double CDB = 0.66 * eps / pow(rad, 2.0);

        explicit ComplexNativeDihedrals(Model const& model):
            NativeDihedralsBase(model) {};

        inline void dihTerm(int i, double phi, double& V, double& dV_dp) const {
            double _phi0 = phi0[i];

            V += CDA * (1.0 - cos(phi - _phi0)) +
                 CDB * (1.0 - cos(3.0 * (phi - _phi0)));

            dV_dp += CDA * sin(phi - _phi0) +
                     3.0 * CDB * sin(3.0 * (phi - _phi0));
        }
    };
}