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

        #pragma omp master
        for (auto const& task : asyncTasks) {
            task();
        }
            
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

void Simulation::step(double t) {
    auto& state = var<State>();
    auto t0 = state.t;
    while (state.t - t0 < t) {
        step();
        if (!checkpoints.empty() && state.t - t0 >= checkpoints.front().first) {
            std::fstream fs;
            fs.open(checkpoints.front().second.data(), std::fstream::out);
            serialize(fs);
            fs.close();
            checkpoints.pop();
        }
    }
}

void Simulation::setCheckpoint(double t, std::string const& filename) {
    checkpoints.emplace(t, filename);
}

void Simulation::serialize(std::ostream &ostream) {
    for (NonlocalForce* f: nonlocalForces) f->serialize(ostream);
    integrator->serialize(ostream);
    ostream << step_nr << '\n';
}

void Simulation::deserialize(std::istream &istream) {
    for (NonlocalForce* f: nonlocalForces) f->deserialize(istream);
    integrator->deserialize(istream);
    istream >> step_nr;
}
