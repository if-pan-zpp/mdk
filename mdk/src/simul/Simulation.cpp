#include "simul/Simulation.hpp"
using namespace mdk;

void Simulation::step() {
    sched.run();
}