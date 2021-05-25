#include "forces/go/NativeContacts.hpp"
#include "kernels/LennardJones.hpp"
using namespace mdk;

void NativeContacts::bind(Simulation &simulation) {
    NonlocalForce::bind(simulation);

    auto& model = simulation.data<Model>();
    for (auto const& cont: model.contacts) {
        if ((ContactTypeIdx)cont.type != ContactTypeIdx::NAT) {
            allContacts.emplace_back((Contact) {
                .i1 = cont.res[0], .i2 = cont.res[1],
                .r_min = cont.dist0
            });
        }
    }

    installIntoVL(false);
}

vl::Spec NativeContacts::spec() const {
    double maxCutoff = 0.0;
    for (auto const& cont: allContacts) {
        auto contCutoff = LennardJones(cont.r_min, depth).cutoff();
        maxCutoff = std::max(maxCutoff, contCutoff);
    }

    return (vl::Spec) {
        .cutoffSq = pow(maxCutoff, 2.0),
        .minBondSep = 3
    };
}

bool operator<(NativeContacts::Contact const& p1, std::pair<int, int> const& p2) {
    return std::make_pair(p1.i1, p1.i2) < p2;
}

bool operator<(std::pair<int, int> const& p1, NativeContacts::Contact const& p2) {
    return p1 < std::make_pair(p2.i1, p2.i2);
}

void NativeContacts::vlUpdateHook() {
    curPairs.clear();
    std::set_intersection(
        allContacts.begin(), allContacts.end(),
        vl->pairs.begin(), vl->pairs.end(),
        std::back_inserter(curPairs));

    newVL.clear();
    std::set_difference(
        vl->pairs.begin(), vl->pairs.end(),
        curPairs.begin(), curPairs.end(),
        std::back_inserter(newVL));

    std::swap(vl->pairs, newVL);
}

void NativeContacts::computeForce() {
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
