#pragma once
#include "DihedralBase.hpp"
#include "../../files/param/Parameters.hpp"

namespace mdk {
    class HeuresticDihedral: public DihedralBase<HeuresticDihedral> {
    private:
        double coeff[numOfPTs][6];
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> angleTypes;

    public:
        void bind(Simulation& simulation) override;
        void dihTerm(int i, double phi, double& V, double& dV_dphi) const;
    };
}