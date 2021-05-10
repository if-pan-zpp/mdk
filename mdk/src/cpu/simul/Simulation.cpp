#include "cpu/simul/Simulation.hpp"
using namespace mdk;
using namespace std;

void Simulation::localPass() {
    for (int i = 0; i+1 < state.n; ++i) {
        int i1 = i-2, i2 = i-1, i3 = i, i4 = i+1;

        if (i3 < 0 || !seqs.isConnected[i3])
            continue;

        Vector r3 = state.r[i3], r4 = state.r[i4], r34 = r4 - r3;
        double norm34 = r34.norm();
        Vector u34 = r34 / norm34;

        if (harm) {
            harm->perPair(i3, u34, norm34, dyn.V, dyn.F[i3], dyn.F[i4]);
        }

        if (i2 < 0 || !seqs.isConnected[i2])
            continue;

        Vector r2 = state.r[i2], r23 = r3 - r2;
        double norm23 = r23.norm();
        Vector u23 = r23 / norm23, r23_x_r34 = r23.cross(r34);

        Vector u_r23_x_r34 = r23_x_r34;
        if (!u_r23_x_r34.isZero())
            u_r23_x_r34.normalize();

        double cos_theta = -u23.dot(u34);
        cos_theta = max(min(cos_theta, 1.0), -1.0);
        if (cos_theta != 0.0) {
            Vector dth_dr2, dth_dr3, dth_dr4;

            dth_dr2 = u34.cross(u_r23_x_r34) / norm23;
            dth_dr4 = u23.cross(u_r23_x_r34) / norm34;
            dth_dr3 = -dth_dr2-dth_dr4;

            double theta = acos(cos_theta), dV_dth = 0.0;
            if (seqs.isNative[i2] && seqs.isNative[i4] && nativeBA) {
                nativeBA->angleTerm(i, theta, dyn.V, dV_dth);
            }
            else if (heurBA) {
                heurBA->angleTerm(i, theta, dyn.V, dV_dth);
            }

            dyn.F[i2] -= dV_dth * dth_dr2;
            dyn.F[i3] -= dV_dth * dth_dr3;
            dyn.F[i4] -= dV_dth * dth_dr4;
        }

        if (pauliExcl) {
            Vector r24 = r4 - r2;
            double norm24_sq = r24.squaredNorm();

            if (norm24_sq <= pauliExcl->cutoff2) {
                double norm24 = sqrt(norm24_sq);
                r24 /= norm24;
                pauliExcl->asForce(r24, norm24, dyn.V, dyn.F[i2], dyn.F[i4]);
            }
        }

        if (quasiAd) {
            quasiAd->n[i3] = r34-r23;
            if (!quasiAd->n[i3].isZero())
                quasiAd->n[i3].normalize();

            quasiAd->h[i3] = u_r23_x_r34;
        }

        if (i1 < 0 || !seqs.isConnected[i1]) continue;

        Vector r1 = state.r[i1];
        Vector r12 = r2 - r1;
        Vector r12_x_r23 = r12.cross(r23);

        if (!r12_x_r23.isZero() && !r23_x_r34.isZero()) {
            double norm_r12_x_r23 = r12_x_r23.norm();
            Vector u_r12_x_r23 = r12_x_r23 / norm_r12_x_r23;

            // TODO: figure out why the fuck this
            Vector dp_dr1 = -u_r12_x_r23 * norm23;
            Vector dp_dr4 = u_r23_x_r34 * norm23;
            Vector df = (-dp_dr1*r12.dot(r23)+dp_dr4*r23.dot(r34))/(norm23*norm23);
            Vector dp_dr2 = -dp_dr1 + df;
            Vector dp_dr3 = -dp_dr4 - df;

            double phi = acos(u_r12_x_r23.dot(u_r23_x_r34));
            if (r12_x_r23.dot(r34) < 0) phi = -phi;

            double dV_dp = 0.0;
            if (seqs.isNative[i1] && seqs.isNative[i4]) {
                if (compNativeDih) {
                    compNativeDih->dihTerm(i, phi, dyn.V, dV_dp);
                }
                else if (simpNativeDih) {
                    simpNativeDih->dihTerm(i, phi, dyn.V, dV_dp);
                }
            }
            else if (heurDih) {
                heurDih->dihTerm(i, phi, dyn.V, dV_dp);
            }

            dyn.F[i1] -= dV_dp * dp_dr1;
            dyn.F[i2] -= dV_dp * dp_dr2;
            dyn.F[i3] -= dV_dp * dp_dr3;
            dyn.F[i4] -= dV_dp * dp_dr4;
        }

        if (chir) {
            Vector r12_x_r34 = r12.cross(r34);
            chir->perQuad(i, r12, r12_x_r23, r12_x_r34, r23_x_r34, dyn);
        }
    }
}

void Simulation::verletPass() {
    factory.update(state);
    for (auto& item: factory.cur) {
        auto r12 = state.top(state.r[item->i2] - state.r[item->i1]);
        auto norm12_sq = r12.squaredNorm();

        if (norm12_sq <= cutoff2) {
            auto norm12 = sqrt(norm12_sq);
            item->unit = r12 / norm12;
            item->norm = norm12;

            auto const& base = item.base();
            if (item.holds<NoContact>()) {
                if (quasiAd && norm12 <= quasiAd->_cutoff)
                    quasiAd->tryForming(item, state);

                if (pid && norm12 <= pid->_cutoff)
                    pid->perPair(base, state, dyn);
            }
            else if (item.holds<qa::Contact>()) {
                quasiAd->perPair(item, state, dyn);
            }
            else if (item.holds<NativeNormal>() && norm12 <= natCont->_cutoff) {
                auto& nn = item.data<NativeNormal>();
                natCont->perNormal(base, nn, dyn);
            }
            else if (item.holds<NativeDisulfide>()) {
                natCont->perDisulfide(base, dyn);
            }

            if (constDH && norm12 <= constDH->_cutoff)
                constDH->perPair(base, dyn);

            if (relativeDH && norm12 <= relativeDH->_cutoff)
                relativeDH->perPair(base, dyn);

            if (pauliExcl && norm12 <= pauliExcl->_cutoff)
                pauliExcl->perPair(base, dyn);
        }
    }
}

Simulation::Simulation(const Model &model):
    factory(*this), state(model), dyn(model.n) {

    seqs = Sequences(model);
}

bool Simulation::coherencyCheck() const {
    if (!(pc.has_value() ^ leapfrog.has_value()))
        return false;

    if (simpNativeDih.has_value() && compNativeDih.has_value())
        return false;

    if (constDH.has_value() && relativeDH.has_value())
        return false;

    if (pid.has_value() && quasiAd.has_value())
        return false;

    if (!pauliExcl)
        return false;

    return true;
}

void Simulation::init() {
    if (!coherencyCheck()) throw;

    factory.update(state);
    cutoff2 = pow(factory.cur.cutoff(), 2.0);
    dyn = Dynamics(state.n);

    if (langDyn) langDyn->initVel(state);
    if (pc) pc->init(state);

    initialized = true;
}

void Simulation::step(int n) {
    if (!initialized)
        init();

    for (int i = 0; i < n && !interrupt; ++i) {
        dyn.reset();
        if (pc) pc->predict(state);

        if (!equilibrationPhase) {
            localPass();
            verletPass();
        }
        if (langDyn) langDyn->eval(state, dyn);

        if (pc) pc->correct(state, dyn);
        if (leapfrog) leapfrog->step(state, dyn);

        for (auto& hook: hooks) {
            hook->execute(*this);
        }
    }
}

void Simulation::step(double t) {
    if (!initialized)
        init();

    double dt = 0.0;
    if (pc) dt = pc->dt;
    else dt = leapfrog->dt;

    step((int)std::floor(t / dt));
}