#include "forces/go/NormalNative.hpp"
#include "kernels/LennardJones.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

void NormalNative::bind(Simulation &simulation) {
    GoBase::bind(simulation);

    auto& model = simulation.data<Model>();
    for (auto const& cont: model.contacts) {
        if ((ContactTypeIdx)cont.type != ContactTypeIdx::NAT) {
            allContacts.emplace_back((Contact) {
                .i1 = cont.res[0], .i2 = cont.res[1],
                .r_min = cont.dist0
            });
        }
    }
}

void NormalNative::run() {
    for (auto const& cont: curPairs) {
        auto r12 = state->top(state->r[cont.i1] - state->r[cont.i2]);
        auto x2 = r12.squaredNorm();
        if (x2 > savedSpec.cutoffSq) continue;

        auto x = sqrt(x2);
        auto unit = r12/x;

        auto lj = LennardJones(cont.r_min, depth);
        lj.asForce(unit, x, state->dyn.V,
            state->dyn.F[cont.i1], state->dyn.F[cont.i2]);
    }
}

vl::Spec NormalNative::spec() const {
    auto baseSpec = GoBase::spec();
    for (auto const& cont: allContacts) {
        auto contCutoff = LennardJones(cont.r_min, depth).cutoff();
        baseSpec.cutoffSq = std::max(baseSpec.cutoffSq, pow(contCutoff, 2.0));
    }
    return baseSpec;
}

std::vector<Target> NormalNative::sat() const {
    auto _sat = Force::sat();
    _sat.insert(_sat.end(), { })
}
