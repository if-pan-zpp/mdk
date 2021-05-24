#include "forces/Force.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

std::vector<Target> Force::req() const {
    return { state->dynReset };
}

std::vector<Target> Force::sat() const {
    return { state->forceAdded };
}

void Force::bind(Simulation &simulation) {
    state = &simulation.var<State>();
}
