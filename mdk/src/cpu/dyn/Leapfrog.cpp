#include "cpu/dyn/Leapfrog.hpp"
using namespace mdk;

Leapfrog::Leapfrog(double dt) {
    this->dt = dt;
}

double Leapfrog::stepTime() const {
    return dt;
}

void Leapfrog::init(Simulation &simul) {
    m = simul.data<Masses>();
    a_prev = Vectors(m->size(), Vector::Zero());
}

void Leapfrog::step(Vectors& r, Vectors& v, double& t, Vectors const& F) {
    for (int i = 0; i < r.size(); ++i) {
        Vector a = F[i] / m[i];
        v[i] += 0.5 * (a_prev[i] + a) * dt;
        r[i] += v[i] * dt + 0.5 * a * dt*dt;
        a_prev[i] = a;
    }
    t += dt;
}
