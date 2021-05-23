#pragma once
#include <forces/dihedral/NativeDihedralBase.hpp>

namespace mdk {
    class SimpleNativeDihedral:
        public NativeDihedralBase<SimpleNativeDihedral> {
    public:
        double CDH = 3.33 * eps/pow(rad, 2);

        explicit SimpleNativeDihedral(Model const& model):
            NativeDihedralBase(model) {};

        void dihTerm(int i, double phi, double& V, double& dV_dphi) const;
    };
}