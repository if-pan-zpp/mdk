#include <cpu/forces/es/ConstDH.hpp>
using namespace mdk;

ConstDH::ConstDH(Model const& model, param::Parameters const& params):
    ESBase(model, params) {};

vl::Spec ConstDH::recomputeSpec() const {
    return (vl::Spec) {
        .cutoffSq = pow(screeningDist, 2.0),
        .minBondSep = 3,
    };
}

void ConstDH::eval(const BaseState &state, BaseDiff &update) const {
    auto coeff = pow(echarge, 2.0) / (4.0 * M_PI * permittivity);

    for (auto const& p: pairs) {
        auto r12 = state.top(state.r[p.i1] - state.r[p.i2]);
        auto x2 = r12.squaredNorm();
        if (x2 > _spec.cutoffSq) continue;

        auto x = sqrt(x2);
        auto unit = r12/x;

        auto V_DH = coeff * exp(-x/screeningDist)/x;
        update.V += V_DH;

        auto dV_dx = -V_DH * (1.0 + x/screeningDist)/x;
        update.F[p.i1] += dV_dx * unit;
        update.F[p.i2] -= dV_dx * unit;
    }
}