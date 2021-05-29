#include "simul/Simulation.hpp"
#include "forces/Force.hpp"
#include "forces/NonlocalForce.hpp"
#include "hooks/Hook.hpp"
#include "system/Integrator.hpp"
#include "verlet/List.hpp"
using namespace mdk;

extern Dynamics thread_dyn;
#pragma omp threadprivate(thread_dyn)
Dynamics thread_dyn;

void Simulation::calcForces() {
    auto& state = var<State>();
    
    auto& vl = var<vl::List>();
    state.prepareDyn();

    vl.check();

    #pragma omp parallel
    {
        thread_dyn.zero(state.n);
            
        for (auto* force: forces) {
            force->asyncPart(thread_dyn);
        }

        #pragma omp critical
        {
            state.updateWithDyn(thread_dyn);
        }
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
