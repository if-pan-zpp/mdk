#include "forces/angle/NativeBondAngle.hpp"
#include "data/BondAngleRanges.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

void NativeBondAngle::bind(Simulation &simulation) {
    Force::bind(simulation);

    ranges = simulation.data<BondAngleRanges>().native;

    auto& model = simulation.data<Model>();
    for (auto const& ch: model.chains) {
        for (auto const& spIdx: ch.structuredParts) {
            auto const& sp = model.structuredParts[spIdx];

            auto istart = ch.start + sp.off;
            auto iend = istart + sp.len;
            for (int i = istart; i < iend; ++i) {
                theta0[i] = sp.angle[i - istart];
            }
        }
    }
}

void NativeBondAngle::bondAngleTerm(int i, double theta, double &V,
    double &dV_dth) const  {

    auto diff = theta - theta0[i];
    V += CBA * diff * diff;
    dV_dth += 2.0 * CBA * diff;
}


