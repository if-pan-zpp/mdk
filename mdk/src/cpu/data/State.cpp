#include "cpu/data/State.hpp"
using namespace mdk;

State::State(const Model &model) {
    n = model.n;
    t = 0.0;
    r = v = Vectors(n);
    top = model.top;
    stats = std::vector<Stat>(n);

    for (int i = 0; i < model.n; ++i) {
        r[i] = model.residues[i].pos;
        v[i] = { 0.0, 0.0, 0.0 };
    }
}

void State::saveTo(Model &model) {
    for (int i = 0; i < n; ++i) {
        model.residues[i].pos = r[i];
    }
    model.top = top;
}