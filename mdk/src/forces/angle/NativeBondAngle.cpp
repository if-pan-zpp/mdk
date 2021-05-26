#include "forces/angle/NativeBondAngle.hpp"
#include "data/BondAngleRanges.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

void NativeBondAngle::bind(Simulation &simulation) {
    Force::bind(simulation);

    auto const& bondAngleRanges = simulation.data<BondAngleRanges>();
    ranges = bondAngleRanges.native;
    theta0 = bondAngleRanges.theta_0;
}

void NativeBondAngle::bondAngleTerm(int i, double theta, double &V,
    double &dV_dth) const  {

    auto diff = theta - theta0[i];
    V += CBA * diff * diff;
    dV_dth += 2.0 * CBA * diff;
}


