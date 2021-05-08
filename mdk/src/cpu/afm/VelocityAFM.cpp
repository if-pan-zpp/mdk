#include "cpu/afm/VelocityAFM.hpp"
using namespace mdk;

void VelocityAFM::add(int idx, Vector r0, Vector v) {
    afms.emplace_back(idx, r0, v);
}

void VelocityAFM::eval(const State &state, Dynamics& dyn) const {
    for (auto const& [idx, r0, v]: afms) {
        auto r_afm = r0 + v * state.t;
        auto diff = r_afm - state.r[idx];
        Vector F_afm = Vector::Zero();
        harm.asForce(diff.normalized(), diff.norm(), dyn.V,
            dyn.F[idx], F_afm);
    }
}
