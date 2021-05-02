#include "cpu/ff/local/LocalFF.hpp"
using namespace mdk;
using namespace std;

LocalFF::LocalFF(const Model &model) {
    spIdx = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>(model.n, -1);
    theta0 = phi0 = Scalars(model.n);

    for (auto const& chain: model.chains) {
        ranges.emplace_back(chain.start, chain.end);
    }

    for (auto const& sp: model.structuredParts) {
        for (int i = sp.off; i < sp.off + sp.len; ++i) {
            spIdx[i] = sp.idx;
            theta0[i] = sp.angle[i-sp.off];
            phi0[i] = sp.angle[i-sp.off];
        }
    }
}

void LocalFF::compute(const State &state, double &V, Vectors &dV_dr) {
    for (auto const& [start, end]: ranges) {
        for (int i = start; i < end-1; ++i) {
            int i1 = i-2, i2 = i-1, i3 = i, i4 = i+1;

            auto r3 = state.r[i3], r4 = state.r[i4];
            auto r34 = r4 - r3;
            auto norm34 = r34.norm();
            auto n34 = r34 / norm34;

            if (harmonic) {
                double dV_dl = 0.0;
                harmonic->kernel(i, norm34, V, dV_dl);
                dV_dr[i3] -= dV_dl * n34;
                dV_dr[i4] += dV_dl * n34;
            }

            if (i2 < start) continue;

            auto r2 = state.r[i2];
            auto r23 = r3 - r2;
            auto norm23 = r23.norm();
            auto n23 = r23 / norm23;

            auto cos_theta = n23.dot(n34);
            cos_theta = max(min(cos_theta, 1.0), -1.0);
            if (cos_theta != 0.0) {
                Vector dth_dr2, dth_dr3, dth_dr4;

                auto n23_x_n34 = n23.cross(n34);
                dth_dr2 = -n34.cross(n23_x_n34) / norm23;
                dth_dr4 = -n23.cross(n23_x_n34) / norm34;
                dth_dr3 = - dth_dr2 - dth_dr4;

                auto theta = acos(cos_theta);
                auto dV_dth = 0.0;

                auto sp2 = spIdx[i2], sp4 = spIdx[i4];
                if (sp2 == sp4 && sp2 >= 0 && nativeBA) {
                    nativeBA->kernel(theta, theta0[i], V, dV_dth);
                }
                else if (tabBA) {
                    tabBA->kernel(i, theta, V, dV_dth);
                }
                else if (heurBA) {
                    heurBA->kernel(i, theta, V, dV_dth);
                }

                dV_dr[i2] += dV_dth * dth_dr2;
                dV_dr[i3] += dV_dth * dth_dr3;
                dV_dr[i4] += dV_dth * dth_dr4;
            }

            if (locExcl) {
                double dV_dl = 0.0, norm24 = 0.0;
                auto r24 = r4 - r2;
                auto norm24_sq = r24.squaredNorm();

                if (locExcl->kernel(norm24_sq, norm24, V, dV_dl)) {
                    r24 /= norm24;
                    dV_dl[i2] -= dV_dl * r24;
                    dV_dl[i3] += dV_dl * r24;
                }
            }

            if (i1 < start) continue;

            auto r1 = state.r[i1];
            auto r12 = r2 - r1;

            auto r12_x_r23 = r12.cross(r23), r23_x_r34 = r23.cross(r34);
            auto norm_r12_x_r23 = r12_x_r23.norm(),
                 norm_r23_x_r34 = r23_x_r34.norm();

            if (norm_r12_x_r23 != 0.0 && norm_r23_x_r34 != 0.0) {
                Vector dp_dr1, dp_dr2, dp_dr3, dp_dr4;

                auto r12_xn_r23 = r12_x_r23 / norm_r12_x_r23;
                auto r23_xn_r34 = r23_x_r34 / norm_r23_x_r34;
                auto cos_phi = r12_xn_r23.dot(r23_xn_r34);
                cos_phi = max(min(cos_phi, 1.0), -1.0);

                auto sgn = -r12.dot(r23_x_r34);
                auto phi = sgn < 0.0 ? -acos(cos_phi) : acos(cos_phi);

                double dV_dp = 0.0;
                auto sp1 = spIdx[i1], sp4 = spIdx[i4];
                if (sp1 == sp4 && sp1 >= 0) {
                    if (compNativeDih) {
                        compNativeDih->kernel(phi, phi0[i], V, dV_dp);
                    }
                    else if (simpNativeDih) {
                        simpNativeDih->kernel(phi, phi0[i], V, dV_dp);
                    }
                }
                else if (heurDih) {
                    heurDih->kernel(i, phi, V, dV_dp);
                }
            }

            if (chir) {
                auto r12_x_r34 = r12.cross(r34);
                chir->kernel(i, r12, r12_x_r23, r12_x_r34, r23_x_r34,
                    V, dV_dr);
            }
        }
    }
}