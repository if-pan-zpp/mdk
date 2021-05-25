#include "system/PredictorCorrector.hpp"
#include "system/State.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

void PredictorCorrector::integrate() {
    for (int i = 0; i < state.n; ++i) {
        y0[i] += y1[i] + y2[i] + y3[i] + y4[i] + y5[i];
        y1[i] += 2.0*y2[i] + 3.0*y3[i] + 4.0*y4[i] + 5.0*y5[i];
        y2[i] += 3.0*y3[i] + 6.0*y4[i] + 10.0*y5[i];
        y3[i] += 4.0*y4[i] + 10.0*y5[i];
        y4[i] += 5.0*y5[i];

        state->r[i] = y0[i];
        state->v[i] = y1[i]/dt;
    }

    for (int i = 0; i < state.n; ++i) {
        Vector err = y2[i] - state.dyn.F[i]/m[i] * (dt*dt/2.0);
        y0[i] -= 3.0/16.0 * err;
        y1[i] -= 251.0/360.0 * err;
        y2[i] -= 1.0 * err;
        y3[i] -= 11.0/18.0 * err;
        y4[i] -= 1.0/6.0 * err;
        y5[i] -= 1.0/60.0 * err;

        state->r[i] = y0[i];
        state->v[i] = y1[i]/dt;
    }

    state->t += dt;
}

void PredictorCorrector::bind(Simulation &simulation) {
    Integrator::bind(simulation);

    m = simulation.data<Masses>();
    auto model = simulation.data<Model>();

    y0 = y1 = y2 = y3 = y4 = y5 = Vectors(model.n, Vector::Zero());
    for (int i = 0; i < model.n; ++i) {
        auto& res = model.residues[i];
        y0[i] = res.r;
        y1[i] = res.v * dt;
    }
}
