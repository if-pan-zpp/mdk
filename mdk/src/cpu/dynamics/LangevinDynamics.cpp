#include "cpu/dynamics/LangevinDynamics.hpp"
using namespace mdk;

void LangevinDynamics::initVel(State &state) {
    for (int i = 0; i < state.n; ++i) {
        state.v[i] = random->sphere() * sqrt(3*kB*T/m[i]);
    }
}

void LangevinDynamics::eval(const State &state, Dynamics &dyn) {
    for (int i = 0; i < state.n; ++i) {
        dyn.F[i] += -gamma*m[i]*state.v[i]
            + sqrt(2*m[i]*gamma*kB*T) * random->sphere();
    }
}
