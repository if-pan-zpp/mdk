#include "cpu/data/Types.hpp"
using namespace mdk;

Types::Types(const Model &model) {
    types = Eigen::Matrix<ResType, Eigen::Dynamic, 1>(model.n);
    for (int i = 0; i < model.n; ++i) {
        types[i] = model.residues[i].type;
    }
}
