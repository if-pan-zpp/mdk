#include "forces/Force.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

void Force::bind(Simulation &simulation) {
    state = &simulation.var<State>();
}

void Force::asyncPart() {}

void Force::syncPart() {}
