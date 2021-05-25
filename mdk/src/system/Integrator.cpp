#include "system/Integrator.hpp"
using namespace mdk;

std::vector<Target *> Integrator::req() {
    return { &state->beforeUpdate };
}

std::vector<Target *> Integrator::sat() {
    return { &state->stateUpdated };
}

void Integrator::run() {
    integrate();
}

void Integrator::bind(Simulation &simulation) {
    state = &simulation.var<State>();
}
