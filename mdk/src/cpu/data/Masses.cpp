#include "cpu/data/Masses.hpp"
#include "utils/AminoAcid.hpp"
#include <unordered_map>
#include "utils/ResidueData.hpp"
#include "cpu/data/Types.hpp"
using namespace mdk;

Masses::Masses(const Model &model) {
    static auto resData = createResData();
    auto types = Types(model);

    mass = Scalars(model.n);
    for (int i = 0; i < model.n; ++i) {
        mass[i] = resData.at((AminoAcid)(int8_t)types[i]).mass;
    }
}

Masses::Masses(const Model &model, double mass0): Masses(model) {
    mass.fill(mass.sum() / mass.size());
}
