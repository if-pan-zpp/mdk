#include "cpu/dyn/PredictorCorrector.hpp"
using namespace mdk;

PredictorCorrector::PredictorCorrector(double dt) {
    this->dt = dt;
}

double PredictorCorrector::stepTime() const {
    return dt;
}

void PredictorCorrector::init(Simulation &simul) {
    m = simul.data<Masses>();
    y0 = y1 = y2 = y3 = y4 = y5 = Vectors(m->size(), Vector::Zero());

    for (int i = 0; i < simul.state.n; ++i) {
        y0[i] = simul.state.r[i];
        y1[i] = simul.state.v[i] * dt;
    }
}

void PredictorCorrector::step(Vectors &r, Vectors &v, double &t,
    const Vectors &F) {

    for (int i = 0; i < r.size(); ++i) {
        Vector err = y2[i] - F[i]/m[i] * (dt*dt/2.0);
        y0[i] -= 3.0/16.0 * err;
        y1[i] -= 251.0/360.0 * err;
        y2[i] -= 1.0 * err;
        y3[i] -= 11.0/18.0 * err;
        y4[i] -= 1.0/6.0 * err;
        y5[i] -= 1.0/60.0 * err;

        r[i] = y0[i];
        v[i] = y1[i]/dt;
    }

    for (int i = 0; i < r.size(); ++i) {
        y0[i] += y1[i] + y2[i] + y3[i] + y4[i] + y5[i];
        y1[i] += 2.0*y2[i] + 3.0*y3[i] + 4.0*y4[i] + 5.0*y5[i];
        y2[i] += 3.0*y3[i] + 6.0*y4[i] + 10.0*y5[i];
        y3[i] += 4.0*y4[i] + 10.0*y5[i];
        y4[i] += 5.0*y5[i];

        r[i] = y0[i];
        v[i] = y1[i]/dt;
    }

    t += dt;
}