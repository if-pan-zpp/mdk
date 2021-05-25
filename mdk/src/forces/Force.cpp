#include "forces/Force.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

std::vector<Target*> Force::req() {
    return { &state->forcesReset };
}

std::vector<Target*> Force::sat() {
    return { &state->beforeUpdate };
}

void Force::bind(Simulation &simulation) {
    state = &simulation.var<State>();
}

void Force::run() {
    computeForce();
}
