#pragma once
#include <forces/dihedral/NativeDihedralBase.hpp>

namespace mdk {
    class ComplexNativeDihedral:
        public NativeDihedralBase<ComplexNativeDihedral> {
    public:
        double CDA = 0.66 * eps / pow(rad, 2.0);
        double CDB = 0.66 * eps / pow(rad, 2.0);

        void dihTerm(int i, double phi, double& V, double& dV_dphi) const;
    };
}