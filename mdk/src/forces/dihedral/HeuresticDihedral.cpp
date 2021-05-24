#include "forces/dihedral/HeuresticDihedral.hpp"
#include "data/DihedralRanges.hpp"
#include "simul/Simulation.hpp"

using namespace mdk;

HeuresticDihedral::HeuresticDihedral(Model const& model,
    const param::Parameters &params) {

}

void HeuresticDihedral::dihTerm(int i, double phi, double &V,
    double &dV_dphi) const {

    auto sin_phi = sin(phi), cos_phi = cos(phi);

    auto* C = coeff[angleTypes[i]];
    V += C[0] + C[1] * sin_phi + C[2] * cos_phi + C[3] * sin_phi * sin_phi +
         C[4] * cos_phi * cos_phi + C[5] * sin_phi * cos_phi;
    dV_dphi += C[1] * cos_phi - C[2] * sin_phi + 2.0 * C[3] * sin_phi -
             2.0 * C[4] * sin_phi + C[5] * (cos_phi - sin_phi);
}

void mdk::HeuresticDihedral::bind(Simulation &simulation) {
    Force::bind(simulation);

    using namespace boost::icl;
    auto& model = *simulation.data<Model>();
    auto& params = *simulation.data<param::Parameters>();

    ranges = simulation.data<DihedralRanges>()->nonNative;

    angleTypes = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>(model.n);
    for (auto const& chain: model.chains) {
        auto start = chain.start + 2;
        auto end = chain.end - 1;

        for (int i = start; i < end; ++i) {
            auto i2 = i-1, i3 = i;
            AminoAcid acid2(int8_t(model.residues[i2].type));
            AminoAcid acid3(int8_t(model.residues[i3].type));

            PairType pt = pairType(acid2, acid3);
            angleTypes[i] = (int8_t)pt;
        }
    }

    for (auto const& [pt, coeffs]: params.dihedralParams) {
        for (int d = 0; d < 6; ++d) {
            coeff[(int8_t)pt][d] = coeffs[d];
        }
    }
}
