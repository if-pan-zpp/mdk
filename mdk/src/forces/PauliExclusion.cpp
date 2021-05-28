#include "forces/PauliExclusion.hpp"
#include <mdk/data/Chains.hpp>
using namespace mdk;

PauliExclusion::PauliExclusion() :
    stlj(5.0 * angstrom, 1.0 * eps) {}

void PauliExclusion::bind(Simulation &simulation) {
    NonlocalForce::bind(simulation);
    chains = &simulation.data<Chains>();
    installIntoVL();
}

vl::Spec PauliExclusion::spec() const {
    return (vl::Spec) {
        .cutoffSq = pow(stlj.r_cut, 2.0),
        .minBondSep = 2,
    };
}

void PauliExclusion::asyncPart() {
    for (auto const& [i1, i2]: exclPairs) {
        if (not (chains -> sepByAtLeastN(i1, i2, 2))) continue;

        auto r12 = state->top(state->r[i1] - state->r[i2]);
        auto x2 = r12.squaredNorm();
        if (x2 > savedSpec.cutoffSq) continue;

        auto x = sqrt(x2);
        auto unit = r12/x;

        stlj.asForce(unit, x, state->dyn.V,
            state->dyn.F[i1], state->dyn.F[i2]);
    }
}

void PauliExclusion::vlUpdateHook() {
    exclPairs = vl->pairs;
}
