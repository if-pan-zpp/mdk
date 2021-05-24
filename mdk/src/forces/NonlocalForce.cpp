#include "forces/NonlocalForce.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

void NonlocalForce::bind(Simulation &simulation) {
    Force::bind(simulation);

    savedSpec = spec();
    vl = &simulation.var<vl::List>();
    vl->registerSpec(savedSpec);
}