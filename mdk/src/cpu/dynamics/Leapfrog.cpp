#include "cpu/dynamics/Leapfrog.hpp"
using namespace mdk;

Leapfrog::Leapfrog(Masses m, double dt) {
    this->m = std::move(m);
    this->dt = dt;
    a_prev = Vectors(this->m.size(), Vector::Zero());
}

void Leapfrog::step(State &state, const Dynamics &dyn) {
    for (int i = 0; i < state.n; ++i) {
        Vector a = dyn.F[i] / m[i];
        state.v[i] += 0.5 * (a_prev[i] + a) * dt;
        state.r[i] += state.v[i] * dt + 0.5 * a * dt*dt;
        a_prev[i] = a;
    }
    state.t += dt;
}
