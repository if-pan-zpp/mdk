#include "cpu/local/HeuresticDihedrals.hpp"
using namespace mdk;

HeuresticDihedrals::HeuresticDihedrals(Model const& model,
        const param::Parameters &params) {
    types = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>(model.n);
    for (auto const& chain: model.chains) {
        for (int i = chain.start+2; i+1 < chain.end; ++i) {
            auto i2 = i-1, i3 = i;
            AminoAcid acid2(int8_t(model.residues[i2].type));
            AminoAcid acid3(int8_t(model.residues[i3].type));

            PairType pt = pairType(acid2, acid3);
            types[i] = (int8_t)pt;
        }
    }

    for (auto const& [pt, coeffs]: params.dihedralParams) {
        for (int d = 0; d < 6; ++d) {
            coeff[(int8_t)pt][d] = coeffs[d];
        }
    }
}