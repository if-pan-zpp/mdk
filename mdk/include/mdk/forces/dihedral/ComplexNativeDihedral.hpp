#pragma once
#include "NativeDihedralBase.hpp"

namespace mdk {
    class ComplexNativeDihedral: public NativeDihedralBase {
    public:
        friend class DihedralAngles;
        double CDA = 0.66 * eps / pow(rad, 2.0);
        double CDB = 0.66 * eps / pow(rad, 2.0);

        void bind(Simulation& simulation) override;

        void term(int i, double phi, double& V, double& dV_dphi) const {
            double _phi0 = phi0[i];

            V += CDA * (1.0 - cos(phi - _phi0)) +
                 CDB * (1.0 - cos(3.0 * (phi - _phi0)));

            dV_dphi += CDA * sin(phi - _phi0) +
                       3.0 * CDB * sin(3.0 * (phi - _phi0));
        }
    };
}