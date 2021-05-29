#include "forces/Tether.hpp"
#include "data/Chains.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;
using namespace boost::icl;

Tether::Tether(bool fromNative) {
    this->fromNative = fromNative;
}

void Tether::bind(Simulation &simulation) {
    Force::bind(simulation);
    dist0 = Scalars::Constant(state->n, 3.8 * angstrom);

    auto model = simulation.data<Model>();
    for (auto const& chain: model.chains) {
        int start = chain.start, end = chain.end-1;
        auto intv = interval<int>::right_open(start, end);
        ranges.add(intv);

        if (fromNative) {
            auto const& residues = model.residues;
            for (auto i = intv.lower(); i < intv.upper(); ++i) {
                if (not residues[i].nat_r || not residues[i+1].nat_r) {
                    throw std::runtime_error("no native position"
                                             " - tether's length can't be calculated");
                }
                Vector r1 = *model.residues[i].nat_r, r2 = *model.residues[i+1].nat_r;
                auto r12 = r2 - r1;
                dist0[i] = r12.norm();
            }
        }
    }
}

void Tether::asyncPart(Dynamics &dyn) {
    for (auto const& intv: ranges) {
        for (int i = intv.lower(); i < intv.upper(); ++i) {
            auto r1 = state->r[i], r2 = state->r[i+1];
            auto r12 = r2 - r1;
            auto r12_norm = r12.norm();

            auto dx = r12_norm - dist0[i];
            auto r12_unit = r12 / r12_norm;
            harm.computeF(r12_unit, dx, dyn.V, dyn.F[i], dyn.F[i+1]);
        }
    }
}
