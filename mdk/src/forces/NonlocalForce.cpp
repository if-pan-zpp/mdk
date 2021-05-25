#include "forces/NonlocalForce.hpp"
#include "simul/Simulation.hpp"
#include "runtime/Lambda.hpp"
using namespace mdk;

void NonlocalForce::bind(Simulation &simulation) {
    Force::bind(simulation);
    vl = &simulation.var<vl::List>();
}

std::vector<Target*> NonlocalForce::req() {
    auto _req = Force::req();
    _req.insert(_req.end(), { &vl->vlChecked });
    return _req;
}

void NonlocalForce::installIntoVL(bool async) {
    savedSpec = spec();
    vl->registerSpec(savedSpec);

    auto updateHookLam = [&]() -> void { vlUpdateHook(); };
    auto updateTask = Lambda({}, updateHookLam, {}).unique();

    auto& hooks = async ?
        vl->updateScheduler.asyncUpdates
        : vl->updateScheduler.destructiveUpdates;

    hooks.emplace_back(std::move(updateTask));
}