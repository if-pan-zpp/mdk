#include "cpu/Simulation.hpp"
using namespace mdk;

void Simulation::run() {
    localPass.init();
    nonlocalPass.init();

    StateDiff sd;
    while (state.t < 1.0) {
        sd.clear();
        localPass.exec(state, sd);
        nonlocalPass.exec(state, sd);
        integrator->step(state, sd);
    }
}