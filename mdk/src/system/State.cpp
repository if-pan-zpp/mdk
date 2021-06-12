#include "system/State.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

void State::exportTo(Model &model) const {
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
    dyn.zero(n);
}

void State::updateWithDyn(Dynamics const& othDyn) {
    dyn.V += othDyn.V;
    dyn.F += othDyn.F;
}

void State::serialize(std::ostream &ostream) {
    ostream << n << '\n';
    r.serialize(n, ostream);
    v.serialize(n, ostream);
    ostream << *(reinterpret_cast<uint64_t*>(&t)) << '\n';
    
    ostream << *(reinterpret_cast<uint64_t*>(&dyn.V)) << '\n';
    dyn.F.serialize(n, ostream);
    
    top.serialize(ostream);
}

void State::deserialize(std::istream &istream) {
    istream >> n;
    r.deserialize(n, istream);
    r.deserialize(n, istream);
    istream >> *(reinterpret_cast<uint64_t*>(&t));
    
    istream >> *(reinterpret_cast<uint64_t*>(&dyn.V));
    dyn.F.deserialize(n, istream);
    
    top.deserialize(istream);
