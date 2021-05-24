#include "forces/NonlocalForce.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

void NonlocalForce::bind(Simulation &simulation) {
    Force::bind(simulation);

    savedSpec = spec();
    vl = &simulation.var<vl::List>();
    vl->registerSpec(savedSpec);
}

std::vector<Target> NonlocalForce::req() const {
    auto _req = Force::req();
    _req.insert(_req.end(), { vl->vlChecked });
    return _req;
}
