#include "forces/LangevinDynamics.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

void LangevinDynamics::bind(Simulation &simulation) {
    Force::bind(simulation);
    m = simulation.data<Masses>();
    random = &simulation.var<Random>();

    for (int i = 0; i < state->n; ++i) {
        state->v[i] = random->sphere() * sqrt(3*kB*T/(*m)[i]);
    }
}

void LangevinDynamics::computeForce() {
    for (int i = 0; i < state->n; ++i) {
        state->dyn.F[i] +=
            -gamma * (*m)[i] * state->v[i]
            + sqrt(2.0 * (*m)[i] * gamma * kB*T) * random->sphere();
    }
}
