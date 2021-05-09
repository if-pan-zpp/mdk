#include "cpu/data/Masses.hpp"
#include "utils/AminoAcid.hpp"
#include "utils/ResidueData.hpp"
#include "cpu/data/Types.hpp"
using namespace mdk;

Masses::Masses(const Model &model) {
    static auto resData = createResData();
    auto types = Types(model);

    resize(model.n);
    for (int i = 0; i < model.n; ++i) {
        (*this)[i] = resData.at((AminoAcid)(int8_t)types[i]).mass;
    }
}
