#include "system/Integrator.hpp"
using namespace mdk;

void Integrator::bind(Simulation &simulation) {
    state = &simulation.var<State>();
}

void Integrator::serialize(std::ostream &ostream) {
    state->serialize(ostream);
}

void Integrator::deserialize(std::istream &istream) {
    state->deserialize(istream);
}
