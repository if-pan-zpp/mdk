#include "system/State.hpp"
#include "runtime/Lambda.hpp"
using namespace mdk;

std::vector<std::unique_ptr<Task>> State::tasks() {
    auto zeroDyn = [&]() -> {
        dyn.V = 0.0;
        dyn.F.vectorwise() = Vector::Zero();
    };
    auto zeroDynTask = Lambda({}, zeroDyn, {dynReset}).unique();

    auto update = [&]() -> {
        integrator->integrate(*this);
    };
    auto updateTask = Lambda({canUpdate}, update, {stateUpdated}).unique();

    return { std::move(zeroDynTask), std::move(updateTask) };
}

void State::updateModel(Model &model) {
    for (int i = 0; i < model.n; ++i) {
        auto& res = model.residues[i];
        res.r = r[i];
        res.v = v[i];
    }
}
