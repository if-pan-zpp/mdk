#include "system/State.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

void State::exportTo(Model &model) {
    for (int i = 0; i < model.n; ++i) {
        auto& res = model.residues[i];
        res.r = r[i];
        res.v = v[i];
    }
}

void State::bind(Simulation &simul) {
    auto& model = simul.data<Model>();

    n = model.n;
    r = v = Vectors(n);
    t = 0.0;
    top = model.top;
    dyn.F = Vectors(n);

    for (int i = 0; i < model.n; ++i) {
        auto& res = model.residues[i];
        r[i] = res.r;
        v[i] = res.v;
    }
}

void State::prepareDyn() {
    dyn.V = 0.0;
    dyn.F.vectorwise() = Vector::Zero();
}
