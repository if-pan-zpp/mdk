#include "data/DihedralRanges.hpp"
using namespace mdk;
using namespace boost::icl;

DihedralRanges::DihedralRanges(const Model &model) {
    phi_0 = Scalars::Zero(model.n);

    for (auto const& ch: model.chains) {
        auto start = ch.start + 2, end = ch.end - 1;
        all.add(interval<int>::right_open(start, end));

        for (auto const& spIdx: ch.structuredParts) {
            auto& sp = model.structuredParts[spIdx];
            auto spStart = ch.start + sp.off + 2;
            auto spEnd = ch.start + sp.off + sp.len - 1;
            native.add(interval<int>::right_open(spStart, spEnd));

            for (int i = spStart; i < spEnd; ++i) {
                phi_0[i] = sp.dihedral[i - (ch.start + sp.off)];
            }
        }
    }

    nonNative = all - native;
}