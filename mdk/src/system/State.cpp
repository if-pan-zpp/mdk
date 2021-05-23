#include "system/State.hpp"
#include "runtime/Lambda.hpp"
using namespace mdk;

std::vector<std::unique_ptr<Task>> State::tasks() {
    auto zeroDyn = [&]() -> {
        dyn.V = 0.0;
        dyn.F.vectorwise() = Vector::Zero();
    };
    auto zeroDynTask = std::make_unique<Lambda<decltype(zeroDyn)>>(
        {}, zeroDyn, {dynReset});

    auto update = [&]() -> {
        integrator->integrate(*this);
    };
    auto updateTask = std::make_unique<Lambda<decltype(update)>>(
        {dynModified}, update, {stateUpdated});

    return { std::move(zeroDynTask), std::move(updateTask) };
}