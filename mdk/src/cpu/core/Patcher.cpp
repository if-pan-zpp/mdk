#include "cpu/core/Patcher.hpp"
using namespace mdk;

void Patcher::patch(State &state, Diff& diff) {
    integrator->step(state.r, state.v, diff.F);
    std::sort(diff.qaDiffs.begin(), diff.qaDiffs.end());

    for (auto const& qaDiff: diff.qaDiffs) {
        auto i1 = qaDiff.cont.i1, i2 = qaDiff.cont.i2;

        auto total1 = state.qaState.stats[i1] + qaDiff.statDiffs[0];
        auto total2 = state.qaState.stats[i2] + qaDiff.statDiffs[1];

        if (qa->checkStat(i1, total1) && qa->checkStat(i2, total2)) {
            if (qaDiff.request == QADiffEntry::Request::FORM) {
                auto& oldPair = state.qaState.freePairs[qaDiff.oldIdx];
                oldPair.status = QAFreePair::Status::TAKEN;
                state.qaState.pairs.emplace_back(qaDiff.cont);
            }
            else if (qaDiff.request == QADiffEntry::Request::BREAK) {
                auto& pair = state.qaState.pairs[qaDiff.oldIdx];
                pair.status = QAContact::Status::BREAKING;
            }
            else {
                auto& oldPair = state.qaState.pairs[qaDiff.oldIdx];
                oldPair.status = QAContact::Status::REMOVED;
                state.qaState.freePairs.emplace_back((QAFreePair) {
                    .i1 = i1, .i2 = i2, .status = QAFreePair::Status::FREE
                });
            }
        }
    }
}