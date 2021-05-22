#include <cpu/forces/afm/ConstantForce.hpp>
using namespace mdk;

std::future<void>
ConstantForce::eval(const State &state, std::vector<Thread *> threads) {
    threads[0]->diff.F[idx] += force;
}

void ConstantForce::init(Simulation &simul) {}
