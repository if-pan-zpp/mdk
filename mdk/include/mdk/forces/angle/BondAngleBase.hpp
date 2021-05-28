#pragma once
#include "../Force.hpp"
using namespace std;

namespace mdk {
    template<typename Impl>
    class BondAngleBase: public Force {
    protected:
        Ranges ranges;

    public:
        void asyncPart() override {
            for (auto const& intv: ranges) {
                for (int i = intv.lower(); i < intv.upper(); ++i) {
                    auto r1 = state->r[i-1], r2 = state->r[i], r3 = state->r[i+1];
                    auto r12 = r2 - r1, r23 = r3 - r2;

                    auto r12_x_r23 = r12.cross(r23);
                    double r12_x_r23_norm = r12_x_r23.norm();
                    if (r12_x_r23_norm != 0.0) {
                        double r12_norm = r12.norm(), r23_norm = r23.norm();

                        /* Changing "Vector" to "auto" gives incorrect results when
                           compiled with -O2 or higher. TODO: whaaat? */
                        Vector dtheta_dr1 = r12.cross(r12_x_r23).normalized() / r12_norm;
                        Vector dtheta_dr3 = r23.cross(r12_x_r23).normalized() / r23_norm;
                        Vector dtheta_dr2 = -dtheta_dr1 - dtheta_dr3;

                        double cos_theta = -r12.dot(r23) / r12_norm / r23_norm;
                        cos_theta = max(min(cos_theta, 1.0), -1.0);
                        double theta = acos(cos_theta), dV_dtheta = 0.0;

                        bondAngleTerm(i, theta, state->dyn.V, dV_dtheta);

                        state->dyn.F[i-1] -= dV_dtheta * dtheta_dr1;
                        state->dyn.F[i] -= dV_dtheta * dtheta_dr2;
                        state->dyn.F[i+1] -= dV_dtheta * dtheta_dr3;
                    }
                }
            }
        }

        void bondAngleTerm(int i, double theta, double& V, double& dV_dtheta) const {
            static_cast<Impl const&>(*this).bondAngleTerm(i, theta, V, dV_dtheta);
        }
    };
}
