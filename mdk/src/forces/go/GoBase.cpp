#include "forces/go/GoBase.hpp"
#include "runtime/Lambda.hpp"
#include <algorithm>
using namespace mdk;

bool operator<(GoBase::Contact const& p1, std::pair<int, int> const& p2) {
    return std::make_pair(p1.i1, p1.i2) < p2;
}

bool operator<(std::pair<int, int> const& p1, GoBase::Contact const& p2) {
    return p1 < std::make_pair(p2.i1, p2.i2);
}

vl::Spec mdk::GoBase::spec() const {
    return (vl::Spec) {
        .cutoffSq = 0.0,
        .minBondSep = 0,
    };
}

void GoBase::bind(Simulation &simulation) {
    NonlocalForce::bind(simulation);

    auto update = [&]() -> {
        curPairs.clear();
        std::set_intersection(
            allContacts.begin(), allContacts.end(),
            vl->pairs.begin(), vl->pairs.end(),
            std::back_inserter(curPairs));

        newVl.clear();
        std::set_difference(
            vl->pairs.begin(), vl->pairs.end(),
            curPairs.begin(), curPairs.end(),
            std::back_inserter(newVl));

        std::swap(vl->pairs, newVl);
    };
    auto updateTask = Lambda({}, update, {}).unique();
    vl->updateScheduler.destructiveUpdates.emplace_back(std::move(updateTask));
}
