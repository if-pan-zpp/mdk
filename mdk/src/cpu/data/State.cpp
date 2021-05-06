#include "cpu/data/State.hpp"
using namespace mdk;

State::State(const Model &model) {
    n = model.n;
    t = 0.0;
    r = v = Vectors(n);
    top = model.top;
    qaStats = qa::Stats(n);
    cysNeigh = Eigen::VectorXi(n);
    cysTaken = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>(n);

    for (int i = 0; i < model.n; ++i) {
        r[i] = model.residues[i].pos;
        v[i] = { 0.0, 0.0, 0.0 };
        cysNeigh[i] = 0;
        cysTaken[i] = 0;
    }
}