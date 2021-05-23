#include <cpu/forces/go/GoBase.hpp>
#include <algorithm>
using namespace mdk;

vl::Spec GoBase::recomputeSpec() const {
    return (vl::Spec) {
        .cutoffSq = 0.0,
        .minBondSep = 0,
    };
}

bool operator<(GoBase::Contact const& p1, std::pair<int, int> const& p2) {
    return std::make_pair(p1.i1, p1.i2) < p2;
}

bool operator<(std::pair<int, int> const& p1, GoBase::Contact const& p2) {
    return p1 < std::make_pair(p2.i1, p2.i2);
}

void GoBase::onVLUpdate(Pairs &vl, BaseState& state) {
    curPairs.clear();
    std::set_intersection(
        allContacts.begin(), allContacts.end(),
        vl.begin(), vl.end(),
        std::back_inserter(curPairs));

    newVl.clear();
    std::set_difference(
        vl.begin(), vl.end(),
        curPairs.begin(), curPairs.end(),
        std::back_inserter(newVl));

    std::swap(vl, newVl);
}
