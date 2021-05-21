#include "cpu/dyn/Dynamics.hpp"
#include "cpu/dyn/PhysicalState.hpp"
using namespace mdk;

void Dynamics::reset() {
    V = 0.0;
    F.colwise() = Vector::Zero();
}

Dynamics& Dynamics::reduce(std::vector<Dynamics *> const& partials) {
    for (int i = 1; i < partials.size(); ++i) {

    }
}

void Dynamics::init(Simulation &simul) {
    auto& phyState = simul.data<PhysicalState>();
    F = Vectors(phyState.n);
}
