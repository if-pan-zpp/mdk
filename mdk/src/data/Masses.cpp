#include "data/Masses.hpp"
#include "utils/AminoAcid.hpp"
#include "utils/ResidueData.hpp"
#include "data/Types.hpp"
using namespace mdk;

Masses::Masses(const Model &model, bool mean) {
    static auto resData = createResData();
    auto types = Types(model);

    resize(model.n);
    for (int i = 0; i < model.n; ++i) {
        (*this)[i] = resData.at((AminoAcid)(int8_t)types[i]).mass;
    }

    if (mean) {
        auto meanMass = this->sum() / model.n;
        for (int i = 0; i < model.n; ++i) {
            (*this)[i] = meanMass;
        }
    }
}
