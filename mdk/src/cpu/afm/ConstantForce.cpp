#include "cpu/afm/ConstantForce.hpp"
using namespace mdk;

void ConstantForce::add(int idx, VRef force) {
    afms.emplace_back(idx, force);
}

void ConstantForce::eval(const State &state, Dynamics &dyn) const {
    for (auto const& [idx, force]: afms) {
        dyn.F[idx] += force;
    }
}
