#include "cpu/nonlocal/DebyeHueckel.hpp"
using namespace mdk;

DebyeHueckel::DebyeHueckel(const Model &model,
        const param::Parameters &params) {
    charge = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>(model.n);

    for (int i = 0; i < model.n; ++i) {
        AminoAcid acid((int8_t)model.residues[i].type);
        auto pol = params.specificity.at(acid).polarization;

        if (pol == param::Polarization::POLAR_POS) {
            charge[i] = 1;
        }
        else if (pol == param::Polarization::POLAR_NEG) {
            charge[i] = -1;
        }
        else {
            charge[i] = 0;
        }
    }
}