#pragma once
#include <mdk/cpu/forces/dihedral/DihedralBase.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class HeuresticDihedral: public DihedralBase<HeuresticDihedral> {
    private:
        double coeff[numOfPTs][6];
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> angleTypes;

    public:
        HeuresticDihedral(Model const& model, param::Parameters const& params);

        void dihTerm(int i, double phi, double& V, double& dV_dphi) const;
    };
}