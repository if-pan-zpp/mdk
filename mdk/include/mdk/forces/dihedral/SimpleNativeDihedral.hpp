#pragma once
#include "NativeDihedralBase.hpp"

namespace mdk {
    class SimpleNativeDihedral: public NativeDihedralBase {
    public:
        friend class DihedralAngles;
        double CDH = 3.33 * eps/pow(rad, 2);

        void bind(Simulation& simulation) override;

        void term(int i, double phi, double& V, double& dV_dphi) const {
            auto diff = phi - phi0[i];
            V += 0.5 * CDH * diff * diff;
            dV_dphi += CDH * diff;
        }
    };
}