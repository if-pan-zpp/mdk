#include "cpu/dynamics/LangevinDynamics.hpp"
using namespace mdk;

LangevinDynamics::LangevinDynamics(Masses const& m, double gamma) {
    this->m = m;
    this->gamma = gamma;
}

void LangevinDynamics::init(Random &random, State &state) {
    for (int i = 0; i < state.n; ++i) {
        state.v[i] = random.sphere() * sqrt(3*kB*T/m[i]);
    }
}

void LangevinDynamics::eval(Random &random, const State &state,
    Dynamics &dyn) {

    for (int i = 0; i < state.n; ++i) {
        dyn.F[i] += -gamma*m[i]*state.v[i]
            + sqrt(2*m[i]*gamma*kB*T) * random.sphere();
    }
}
