#include "system/Leapfrog.hpp"
#include "system/State.hpp"
using namespace mdk;

void Leapfrog::integrate(State &state) {
    for (int i = 0; i < state.n; ++i) {
        Vector a_cur = state.dyn.F[i] / m[i];
        state.v[i] += 0.5 * (a_prev[i] + a_cur) * dt;
        state.r[i] += state.v[i] * dt + 0.5 * a_cur * dt * dt;
        a_prev[i] = a_cur;
    }
    state.t += dt;
}