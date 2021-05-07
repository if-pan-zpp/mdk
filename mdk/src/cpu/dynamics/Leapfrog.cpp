#include "cpu/dynamics/Leapfrog.hpp"
using namespace mdk;

Leapfrog::Leapfrog(const Masses &m, double dt) {
    this->m = m;
    this->dt = dt;
}

void Leapfrog::init(State &state) {
    a_prev = Vectors(state.n, Vector::Zero());
}

void Leapfrog::step(State &state, const Dynamics &dyn) {
    for (int i = 0; i < state.n; ++i) {
        auto a = dyn.F[i] / m[i];
        state.v[i] += 0.5 * (a_prev[i] + a) * dt;
        state.r[i] += state.v[i] * dt + 0.5 * a * dt*dt;
        a_prev[i] = a;
    }
}
