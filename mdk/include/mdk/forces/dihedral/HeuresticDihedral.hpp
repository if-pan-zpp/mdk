#pragma once
#include "../../files/param/Parameters.hpp"
#include "../../simul/SimulVar.hpp"
#include "../../simul/Simulation.hpp"

namespace mdk {
    class HeuresticDihedral: public SimulVar {
    private:
        friend class DihedralAngles;
        double coeff[numOfPTs][6];
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> angleTypes;

    public:
        void bind(Simulation& simulation) override;

        void term(int i, double phi, double& V, double& dV_dphi) const {
            auto sin_phi = sin(phi), cos_phi = cos(phi);

            auto* C = coeff[angleTypes[i]];

            V += C[0] + C[1] * sin_phi + C[2] * cos_phi + C[3] * sin_phi * sin_phi +
                 C[4] * cos_phi * cos_phi + C[5] * sin_phi * cos_phi;

            dV_dphi += C[1] * cos_phi - C[2] * sin_phi + 2.0 * C[3] * sin_phi -
                       2.0 * C[4] * sin_phi + C[5] * (cos_phi - sin_phi);
        }
    };
}