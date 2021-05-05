#pragma once
#include <mdk/tools/param/Parameters.hpp>
#include <Eigen/Core>
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class HeuresticDihedrals {
    private:
        double coeff[numOfPTs][6];
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> types;

    public:
        HeuresticDihedrals(Model const& model,
            param::Parameters const& params);

        void dihTerm(int i, double phi, double& V, double &dV_dp) const;
    };

    inline void HeuresticDihedrals::dihTerm(int i,
        double phi, double &V, double &dV_dp) const {
        auto sin_phi = sin(phi), cos_phi = cos(phi);

        auto* C = coeff[types[i]];
        V += C[0] + C[1] * sin_phi + C[2] * cos_phi + C[3] * sin_phi * sin_phi +
            C[4] * cos_phi * cos_phi + C[5] * sin_phi * cos_phi;
        dV_dp += C[1] * cos_phi - C[2] * sin_phi + 2.0 * C[3] * sin_phi -
            2.0 * C[4] * sin_phi + C[5] * (cos_phi - sin_phi);
    }
}
