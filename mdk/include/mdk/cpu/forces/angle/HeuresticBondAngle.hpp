#pragma once
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/forces/angle/BondAngleBase.hpp>

namespace mdk {
    class HeuresticBondAngle: public BondAngleBase<HeuresticBondAngle> {
    private:
        static constexpr const int D = 6;
        double coeff[numOfPTs][D+1];
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> angleTypes;

    public:
        HeuresticBondAngle(Model const& model, param::Parameters const& params);

        void bondAngleTerm(int i, double theta, double& V, double& dV_dth) const;
    };
}