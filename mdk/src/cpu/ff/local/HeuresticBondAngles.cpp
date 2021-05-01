#include "cpu/ff/local/HeuresticBondAngles.hpp"
using namespace mdk;

HeuresticBondAngles::HeuresticBondAngles(Model const& model,
        const param::Parameters &params) {
    types = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>(model.n);
    for (auto const& [start, end]: model.chains) {
        for (int i = start+1; i+1 < end; ++i) {
            auto i1 = i-1, i2 = i, i3 = i+1;
            auto type2 = model.residues[i2].type;
            auto type3 = model.residues[i3].type;

            if (AminoAcid::isProper(type2) && AminoAcid::isProper(type3)) {
                AminoAcid acid2(type2), acid3(type3);
                PairType pt = pairType(acid2, acid3);
                types[i] = (int8_t)pt;
            }
        }
    }

    for (auto const& [pt, coeffs]: params.angleParams) {
        for (int d = 0; d < 7; ++d) {
            coeff[(int8_t)pt][d] = coeffs[d];
        }
    }
}