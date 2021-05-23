#include "verlet/Factory.hpp"
#include <Eigen/Geometry>
using namespace mdk;
using namespace mdk::vl;


void Factory::genPairList() {
//    prepCells();
//    cellLoop();
//    std::sort(pairs.begin(), pairs.end());
    auto effCutoff = cutoff + 2.0 * pad;
    auto effCutoffSq = pow(effCutoff, 2.0);

    for (int pt1 = 0; pt1 < r0.cols(); ++pt1) {
        auto r1 = state0->r[pt1];
        for (int pt2 = pt1+1; pt2 < r0.cols(); ++pt2) {
            auto r2 = state0->r[pt2];
            auto r12_norm2 = state0->top(r2 - r1).squaredNorm();
            if (r12_norm2 <= effCutoffSq)
                pairs.emplace_back(pt1, pt2);
        }
    }
}

bool Factory::needToReset(BaseState const& state) const {
    if (&state != state0) return true;
    if (t0 == state.t) return false;

    auto maxMoveSq = 0.0;
    for (int i = 0; i < state0->n; ++i) {
        auto moveSq = (state0->r[i] - r0[i]).squaredNorm();
        maxMoveSq = std::max(maxMoveSq, moveSq);
    }

    auto maxMove = sqrt(maxMoveSq);
    auto pbcShift = (top0.cell - state0->top.cell).lpNorm<1>();
    return maxMove + 2.0 * pbcShift >= pad;
}

bool Factory::update(BaseState const& state) {
    auto reset = needToReset(state);

    if (needToReset(state)) {
        this->state0 = &state;
        t0 = state.t;
        r0 = state.r;
        top0 = state.top;
        genPairList();
    }
    return reset;
}