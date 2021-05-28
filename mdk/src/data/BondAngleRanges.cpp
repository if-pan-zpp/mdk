#include "data/BondAngleRanges.hpp"
using namespace mdk;
using namespace boost::icl;

BondAngleRanges::BondAngleRanges(const Model &model) {
    theta_0 = Scalars::Zero(model.n);

    for (auto const& ch: model.chains) {
        auto start = ch.start + 1, end = ch.end - 1;
        all.add(interval<int>::right_open(start, end));

        for (auto const& spIdx: ch.structuredParts) {
            auto& sp = model.structuredParts[spIdx];
            auto spStart = ch.start + sp.off + 1;
            auto spEnd = ch.start + sp.off + sp.len - 1;
            native.add(interval<int>::right_open(spStart, spEnd));

            for (int i = spStart; i < spEnd; ++i) {
                theta_0[i] = sp.angle[i - (ch.start + sp.off)];
            }
        }
    }

    nonNative = all - native;
}