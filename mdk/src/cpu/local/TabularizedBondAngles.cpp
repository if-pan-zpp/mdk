#include "cpu/local/TabularizedBondAngles.hpp"
using namespace mdk;

TabularizedBondAngles::TabularizedBondAngles(Model const& model,
        const tab::TabEnergy &tabEnergy) {
    types = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>(model.n);
    for (auto const& chain: model.chains) {
        for (int i = chain.start+1; i+1 < chain.end; ++i) {
            auto i2 = i, i3 = i+1;
            auto type2 = model.residues[i2].type;
            auto type3 = model.residues[i3].type;

            if (AminoAcid::isProper(type2) && AminoAcid::isProper(type3)) {
                AminoAcid acid2(type2), acid3(type3);
                PairType pt = pairType(acid2, acid3);
                types[i] = (int8_t)pt;
            }
        }
    }

    for (auto const& [pt, vals]: tabEnergy.angleV) {
        auto& tab = values[(int8_t)pt];

        tab.n = vals.values.size();
        tab.values = Scalars(tab.n);
        for (int i = 0; i < tab.n; ++i) {
            tab.values[i] = vals.values[i];
        }
    }
}