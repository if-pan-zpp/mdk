#include "cpu/forces/go/NormalNative.hpp"
#include "cpu/kernels/LennardJones.hpp"
using namespace mdk;

NormalNative::NormalNative(const Model &model) {
    for (auto const& cont: model.contacts) {
        if ((ContCode)cont.type != ContCode::NAT) {
            allContacts.emplace_back((Contact) {
                .i1 = cont.res[0], .i2 = cont.res[1],
                .r_min = cont.dist0
            });
        }
    }
}

vl::Spec NormalNative::recomputeSpec() const {
    auto baseSpec = GoBase::recomputeSpec();
    for (auto const& cont: allContacts) {
        auto contCutoff = LennardJones(cont.r_min, depth).cutoff();
        _spec.cutoffSq = std::max(_spec.cutoffSq, pow(contCutoff, 2.0));
    }
    return baseSpec;
}

void NormalNative::eval(const BaseState &state, BaseDiff &update) const {
    for (auto const& cont: curPairs) {
        auto r12 = state.top(state.r[cont.i1] - state.r[cont.i2]);
        auto x2 = r12.squaredNorm();
        if (x2 > _spec.cutoffSq) continue;

        auto x = sqrt(x2);
        auto unit = r12/x;

        auto lj = LennardJones(cont.r_min, depth);
        lj.asForce(unit, x, update.V, update.F[cont.i1], update.F[cont.i2]);
    }
}
