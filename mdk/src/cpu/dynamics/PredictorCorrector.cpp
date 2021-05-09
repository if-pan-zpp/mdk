#include "cpu/dynamics/PredictorCorrector.hpp"
using namespace mdk;

PredictorCorrector::PredictorCorrector(Masses m, double dt):
    m(std::move(m)){

    this->dt = dt;
    auto n = this->m.size();
    y0 = y1 = y2 = y3 = y4 = y5 = Vectors(n, Vector::Zero());
}

void PredictorCorrector::init(State &state) {
    for (int i = 0; i < state.n; ++i) {
        y0[i] = state.r[i];
        y1[i] = state.v[i] * dt;
    }
}

void PredictorCorrector::predict(State &state) {
    state.t += dt;
    for (int i = 0; i < state.n; ++i) {
        y0[i] += y1[i] + y2[i] + y3[i] + y4[i] + y5[i];
        y1[i] += 2.0*y2[i] + 3.0*y3[i] + 4.0*y4[i] + 5.0*y5[i];
        y2[i] += 3.0*y3[i] + 6.0*y4[i] + 10.0*y5[i];
        y3[i] += 4.0*y4[i] + 10.0*y5[i];
        y4[i] += 5.0*y5[i];

        state.r[i] = y0[i];
        state.v[i] = y1[i]/dt;
    }
}

void PredictorCorrector::correct(State &state, const Dynamics &dyn) {
    for (int i = 0; i < state.n; ++i) {
        Vector err = y2[i] - dyn.F[i]/m[i] * (dt*dt/2.0);
        y0[i] -= 3.0/16.0 * err;
        y1[i] -= 251.0/360.0 * err;
        y2[i] -= 1.0 * err;
        y3[i] -= 11.0/18.0 * err;
        y4[i] -= 1.0/6.0 * err;
        y5[i] -= 1.0/60.0 * err;

        state.r[i] = y0[i];
        state.v[i] = y1[i]/dt;
    }
}