#include "cpu/forces/PauliExclusion.hpp"
using namespace mdk;

void PauliExclusion::updateVL(Pairs &vl, BaseState const& state) {
    exclPairs = vl;
}

vl::Spec PauliExclusion::recomputeSpec() const {
    return (vl::Spec) {
        .cutoffSq = pow(stlj.r_cut, 2.0),
        .minBondSep = 2,
    };
}

void PauliExclusion::eval(const BaseState &state, BaseDiff &update) const {
    for (auto const& [i1, i2]: exclPairs) {
        auto r12 = state.top(state.r[i1] - state.r[i2]);
        auto x2 = r12.squaredNorm();
        if (x2 > _spec.cutoffSq) continue;

        auto x = sqrt(x2);
        auto unit = r12/x;

        stlj.asForce(unit, x, update.V, update.F[i1], update.F[i2]);
    }
}