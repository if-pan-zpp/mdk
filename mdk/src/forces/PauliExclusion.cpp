#include "forces/PauliExclusion.hpp"
#include "runtime/Lambda.hpp"
using namespace mdk;

void PauliExclusion::bind(Simulation &simulation) {
    NonlocalForce::bind(simulation);

    auto update = [this]() -> void { updateLocalVL(); };
    auto updateTask = Lambda({}, update, {}).unique();
    vl->updateScheduler.asyncUpdates.emplace_back(std::move(updateTask));
}

vl::Spec PauliExclusion::spec() const {
    return (vl::Spec) {
        .cutoffSq = pow(stlj.r_cut, 2.0),
        .minBondSep = 2,
    };
}

void PauliExclusion::run() {
    for (auto const& [i1, i2]: exclPairs) {
        auto r12 = state->top(state->r[i1] - state->r[i2]);
        auto x2 = r12.squaredNorm();
        if (x2 > savedSpec.cutoffSq) continue;

        auto x = sqrt(x2);
        auto unit = r12/x;

        stlj.asForce(unit, x, state->dyn.V, state->dyn.F[i1], state->dyn.F[i2]);
    }
}

void PauliExclusion::updateLocalVL() {
    exclPairs = vl->pairs;
}
