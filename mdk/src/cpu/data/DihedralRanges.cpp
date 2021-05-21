#include <cpu/data/DihedralRanges.hpp>
using namespace mdk;
using namespace boost::icl;

DihedralRanges::DihedralRanges(const Model &model) {
    for (auto const& ch: model.chains) {
        auto start = ch.start + 2, end = ch.end - 1;
        all.add(interval<int>::right_open(start, end));

        for (auto const& spIdx: ch.structuredParts) {
            auto& sp = model.structuredParts[spIdx];
            auto spStart = ch.start + sp.off + 2;
            auto spEnd = ch.start + sp.off + sp.len - 1;
            native.add(interval<int>::right_open(spStart, spEnd));
        }
    }

    nonNative = all - native;
}