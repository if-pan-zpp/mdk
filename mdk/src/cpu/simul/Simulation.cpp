#include "cpu/simul/Simulation.hpp"
#include <iostream>
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

            thread_local auto excl2 = pow(pauliExcl->cutoff(), 2);
            if (norm24_sq <= excl2) {
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
    vl.update();

    for (auto& x: vl.freePairs) {
        if (x.status == vl::Free::Status::TAKEN)
            continue;

        auto r12 = state.top(state.r[x.i2] - state.r[x.i1]);
        auto norm12_sq = r12.squaredNorm();

        if (norm12_sq <= cutoff_sq) {
            auto norm12 = sqrt(norm12_sq);
            x.unit = r12 / norm12;
            x.norm = norm12;

            if (quasiAd) quasiAd->tryForming(x, state);
            if (pid) pid->perPair(x, state, dyn);
            if (pauliExcl) pauliExcl->perPair(x, dyn);

            if (constDH) constDH->perPair(x, dyn);
            if (relativeDH) relativeDH->perPair(x, dyn);
        }
    }

    if (natCont) {
        for (auto& x: vl.nativeContacts) {
            auto r12 = state.top(state.r[x.i2] - state.r[x.i1]);
            auto norm12_sq = r12.squaredNorm();

            if (norm12_sq < 4.0 * x.r_min * x.r_min) {
                auto norm12 = sqrt(norm12_sq);
                x.unit = r12 / norm12;
                x.norm = norm12;

                natCont->perNormal(x, dyn);
            }

            if (pauliExcl) pauliExcl->perPair(x, dyn);
        }
    }

    if (quasiAd) {
        for (auto& x: vl.qaContacts) {
            if (x.status == QAContact::Status::VACATED) {
                vl::Free f;
                f.i1 = x.i1; f.i2 = x.i2;
                f.status = vl::Free::Status::FREE;
                vl.freePairs.emplace_back(std::move(f));

                x.status = QAContact::Status::REMOVED;
            }
            else if (x.status != QAContact::Status::REMOVED) {
                auto r12 = state.top(state.r[x.i2] - state.r[x.i1]);
                auto norm12_sq = r12.squaredNorm();

                if (norm12_sq <= cutoff_sq) {
                    auto norm12 = sqrt(norm12_sq);
                    x.unit = r12 / norm12;
                    x.norm = norm12;

                    quasiAd->perPair(x, state, dyn);
                }
            }

            if (pauliExcl) pauliExcl->perPair(x, dyn);
        }
    }
}

Simulation::Simulation(const Model &model):
    vl(*this), state(model), dyn(model.n) {

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

//    if (!pauliExcl)
//        return false;

    return true;
}

void Simulation::init() {
    if (!coherencyCheck()) throw;

    vl.init();
    cutoff_sq = pow(vl.cutoff(), 2.0);
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