#include "cpu/forces/Tether.hpp"
#include "cpu/data/Chains.hpp"
using namespace mdk;
using namespace boost::icl;

Tether::Tether(bool fromNative) {
    this->fromNative = fromNative;
}

void Tether::init(Simulation &simul) {
    dist0 = Scalars::Constant(simul.state.n, 3.8 * angstrom);

    auto& model = simul.data<Model>();
    for (auto const& chain: model->chains) {
        int start = chain.start, end = chain.end-1;
        auto intv = interval<int>::right_open(start, end);
        ranges.add(intv);

        if (fromNative) {
            for (auto i = intv.lower(); i < intv.upper(); ++i) {
                auto i1 = i, i2 = i+1;
                auto r1 = model->residues[i1].pos,
                     r2 = model->residues[i2].pos;
                auto r12 = r2 - r1;

                dist0[i1] = r12.norm();
            }
        }
    }
}

std::future<void>
Tether::eval(const State &state, std::vector<Thread *> threads) {
    auto& diff = threads[0]->diff;
    for (auto const& intv: ranges) {
        for (int i = intv.lower(); i < intv.upper(); ++i) {
            auto r1 = state.r[i], r2 = state.r[i+1];
            auto r12 = r2 - r1;
            auto r12_norm = r12.norm();

            auto dx = r12_norm - dist0[i];
            auto r12_unit = r12 / r12_norm;
            harm.computeF(r12_unit, dx, diff.V, diff.F[i], diff.F[i+1]);
        }
    }
}
