#include "cpu/local/HeuresticBondAngles.hpp"
using namespace mdk;

HeuresticBondAngles::HeuresticBondAngles(Model const& model,
        const param::Parameters &params) {
    types = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>(model.n);
    for (auto const& chain: model.chains) {
        for (int i = chain.start+1; i+1 < chain.end; ++i) {
            auto i2 = i, i3 = i+1;
            AminoAcid acid2(int8_t(model.residues[i2].type));
            AminoAcid acid3(int8_t(model.residues[i3].type));

            PairType pt = pairType(acid2, acid3);
            types[i] = (int8_t)pt;
        }
    }

    for (auto const& [pt, coeffs]: params.angleParams) {
        for (int d = 0; d < 7; ++d) {
            coeff[(int8_t)pt][d] = coeffs[d];
        }
    }
}