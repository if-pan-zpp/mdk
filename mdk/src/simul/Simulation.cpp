#include "simul/Simulation.hpp"
#include "forces/Force.hpp"
#include "forces/NonlocalForce.hpp"
#include "hooks/Hook.hpp"
#include "system/Integrator.hpp"
#include "verlet/List.hpp"
using namespace mdk;

void Simulation::calcForces() {
    auto& state = var<State>();
    
    auto& vl = var<vl::List>();
    state.prepareDyn();

    vl.check();
    for (auto* force: forces) {
        force->asyncPart(state.dyn);
    }

    for (auto* force: forces) {
        force->syncPart(state.dyn);
    }
}

void Simulation::init() {
    step_nr = 0;

    calcForces();
    integrator->init();

    for (auto* hook: hooks) {
        hook->execute(0);
    }
    
    initialized = true;
}

void Simulation::step() {
    if (not initialized) {
        init();
    }
    
    step_nr++;

    calcForces();
    integrator->integrate();

    for (auto* hook: hooks) {
        hook->execute(step_nr);
    }
}
