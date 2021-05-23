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
    auto updateTask = Lambda({dynModified}, update, {stateUpdated}).unique();

    return { std::move(zeroDynTask), std::move(updateTask) };
}