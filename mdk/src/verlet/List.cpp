#include "verlet/List.hpp"
#include "simul/Simulation.hpp"
#include "forces/NonlocalForce.hpp"
using namespace mdk;
using namespace mdk::vl;

void List::update() {
    auto effCutoff = cutoff + 2.0 * pad;
    auto effCutoffSq = pow(effCutoff, 2.0);

    pairs.clear();
    for (int pt1 = 0; pt1 < r0.size(); ++pt1) {
        auto r1 = state->r[pt1];
        for (int pt2 = pt1+1; pt2 < r0.size(); ++pt2) {
            auto r2 = state->r[pt2];
            auto r12_norm2 = state->top(r2 - r1).squaredNorm();

            bool cond = r12_norm2 <= effCutoffSq &&
                chains->sepByAtLeastN(pt1, pt2, minBondSep);

            if (cond) pairs.emplace_back(pt1, pt2);
        }
    }

    for (auto& force: forces) {
        force->vlUpdateHook();
    }
}

bool List::needToReset() const {
    if (initial) return true;
    if (t0 == state->t) return false;

    auto maxMoveSq = 0.0;
    for (int i = 0; i < state->n; ++i) {
        auto moveSq = (state->r[i] - r0[i]).squaredNorm();
        maxMoveSq = std::max(maxMoveSq, moveSq);
    }

    auto maxMove = sqrt(maxMoveSq);
    auto pbcShift = (top0.cell - state->top.cell).lpNorm<1>();
    return maxMove + 2.0 * pbcShift >= pad;
}

void List::bind(Simulation &simulation) {
    state = &simulation.var<State>();
    chains = &simulation.data<Chains>();
    initial = true;
}

void List::check() {
    if (needToReset()) {
        t0 = state->t;
        r0 = state->r;
        top0 = state->top;
        update();
    }
    initial = false;
}

void List::registerNF(NonlocalForce& force, Spec const& spec) {
    cutoff = std::max(cutoff, sqrt(spec.cutoffSq));
    if (minBondSep < 1) minBondSep = spec.minBondSep;
    else minBondSep = std::min(minBondSep, spec.minBondSep);

    forces.push_back(&force);
}
