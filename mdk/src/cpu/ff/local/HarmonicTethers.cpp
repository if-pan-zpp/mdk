#include "cpu/ff/local/HarmonicTethers.hpp"
using namespace mdk;

HarmonicTethers::HarmonicTethers(const Model &model, bool fromNative) {
    dist0 = Scalars::Constant(model.n, 3.8 * angstrom);
    if (fromNative) {
        for (auto const& chain: model.chains) {
            for (int i = chain.start; i < chain.end-1; ++i) {
                auto i1 = i, i2 = i+1;
                auto r1 = model.residues[i1].pos,
                     r2 = model.residues[i2].pos;
                auto r12 = r2 - r1;

                dist0[i1] = r12.norm();
            }
        }
    }
}