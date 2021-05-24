#pragma once
#include "../Force.hpp"
using namespace std;

namespace mdk {
    template<typename Impl>
    class BondAngleBase: public Force {
    protected:
        Ranges ranges;

    public:
        void run() override {
            for (auto const& intv: ranges) {
                for (int i = intv.lower(); i < intv.upper(); ++i) {
                    auto r1 = state->r[i-1], r2 = state->r[i], r3 = state->r[i+1];
                    auto r12 = r2 - r1, r23 = r3 - r2;

                    auto r12_x_r23 = r12.cross(r23);
                    auto r12_x_r23_norm = r12_x_r23.norm();
                    if (r12_x_r23_norm != 0.0) {
                        auto r12_norm = r12.norm(), r23_norm = r23.norm();
                        auto r12_unit = r12 / r12_norm, r23_unit = r23 / r23_norm;
                        auto r12_x_r23_unit = r12_x_r23 / r12_x_r23_norm;

                        auto dtheta_dr1 = r23_unit.cross(r12_x_r23_unit) / r12_norm;
                        auto dtheta_dr3 = r12_unit.cross(r12_x_r23_unit) / r23_norm;
                        auto dtheta_dr2 = -dtheta_dr1 -dtheta_dr3;

                        auto cos_theta = -r12_unit.dot(r23_unit);
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