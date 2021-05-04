#include "cpu/ff/ForceField.hpp"
using namespace mdk;
using namespace std;

void ForceField::localPass(const State &state, StateDiff &sd) const {
    for (auto const& [start, end]: chains) {
        for (int i = start; i < end-1; ++i) {
            int i1 = i-2, i2 = i-1, i3 = i, i4 = i+1;

            Vector r3 = state.r[i3], r4 = state.r[i4], r34 = r4 - r3;
            double norm34 = r34.norm();
            Vector u34 = r34 / norm34;
            int8_t sp4 = spIdx[i4];

            if (harm) {
                harm->eval(i3, u34, norm34, sd.V, sd.dV_dr);
            }

            if (i2 < start) continue;

            Vector r2 = state.r[i2], r23 = r3 - r2;
            double norm23 = r23.norm();
            Vector u23 = r23 / norm23, r23_x_r34 = r23.cross(r34),
                u23_x_u34 = r23_x_r34 / (norm23 * norm34);
            int8_t sp2 = spIdx[i2];

            double cos_theta = u23.dot(u34);
            cos_theta = max(min(cos_theta, 1.0), -1.0);
            if (cos_theta != 0.0) {
                Vector dth_dr2, dth_dr3, dth_dr4;

                dth_dr2 = -u34.cross(u23_x_u34) / norm23;
                dth_dr4 = -u23.cross(u23_x_u34) / norm34;
                dth_dr3 = -dth_dr2-dth_dr4;

                double theta = acos(cos_theta), dV_dth = 0.0;
                if (sp2 == sp4 && sp2 >= 0 && nativeBA) {
                    nativeBA->angleTerm(i, theta, sd.V, dV_dth);
                }
                else if (heurBA) {
                    heurBA->angleTerm(i, theta, sd.V, dV_dth);
                }

                sd.dV_dr[i2] += dV_dth * dth_dr2;
                sd.dV_dr[i3] += dV_dth * dth_dr3;
                sd.dV_dr[i4] += dV_dth * dth_dr4;
            }

            if (pauliExcl) {
                Vector r24 = r4 - r2;
                double norm24_sq = r24.squaredNorm();

                if (norm24_sq <= pauliExcl->cutoff2) {
                    double norm24 = sqrt(norm24_sq);
                    r24 /= norm24;
                    pauliExcl->eval(r24, norm24, sd.V,
                        sd.dV_dr[i2], sd.dV_dr[i3]);
                }
            }

            if (i1 < start) continue;

            Vector r1 = state.r[i1];
            int8_t sp1 = spIdx[i1];
            Vector r12 = r2 - r1;
            Vector r12_x_r23 = r12.cross(r23);
            double norm_r12_x_r23 = r12_x_r23.norm();

            if (!r12_x_r23.isZero() && !r23_x_r34.isZero()) {
                Vector u_r12_x_r23 = r12_x_r23 / norm_r12_x_r23;

                double norm_r23_x_r34 = r23_x_r34.norm();
                Vector u_r23_x_r34 = r23_x_r34 / norm_r23_x_r34;

                // TODO: check whether it's correct; taken from cg.f
                Vector dp_dr1 = -u_r12_x_r23 * norm23;
                Vector dp_dr4 = u_r23_x_r34 * norm23;
                Vector df = (-dp_dr1*r12.dot(r23)+dp_dr4*r23.dot(r34))/(norm23*norm23);
                Vector dp_dr2 = -dp_dr1 + df;
                Vector dp_dr3 = -dp_dr4 - df;

                double cos_phi = u_r12_x_r23.dot(u_r23_x_r34);
                double sgn = -r12.dot(r23_x_r34);
                auto phi = sgn < 0.0 ? -acos(cos_phi) : acos(cos_phi);

                double dV_dp = 0.0;
                if (sp1 == sp4 && sp1 >= 0) {
                    if (compNativeDih) {
                        compNativeDih->dihTerm(i, phi, sd.V, dV_dp);
                    }
                    else if (simpNativeDih) {
                        simpNativeDih->dihTerm(i, phi, sd.V, dV_dp);
                    }
                }
                else if (heurDih) {
                    heurDih->dihTerm(i, phi, sd.V, dV_dp);
                }

                sd.dV_dr[i1] += dV_dp * dp_dr1;
                sd.dV_dr[i2] += dV_dp * dp_dr2;
                sd.dV_dr[i3] += dV_dp * dp_dr3;
                sd.dV_dr[i4] += dV_dp * dp_dr4;
            }

            if (chir) {
                Vector r12_x_r34 = r12.cross(r34);
                chir->eval(i, r12, r12_x_r23, r12_x_r34, r23_x_r34, sd.V, sd.dV_dr);
            }
        }
    }
}

ForceField::ForceField(const Model &model) {
    spIdx.resize(model.n);

    for (auto const& ch: model.chains) {
        for (auto const& curSpIdx: ch.structuredParts) {
            auto const& sp = model.structuredParts[curSpIdx];

            auto istart = ch.start + sp.off;
            auto iend = istart + sp.len;
            for (int i = istart; i < iend; ++i) {
                spIdx[i] = curSpIdx;
            }
        }
    }
}

void ForceField::eval(const State &state, StateDiff &sd) const {
    localPass(state, sd);
}