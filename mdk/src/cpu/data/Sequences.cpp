#include "cpu/data/Sequences.hpp"
using namespace mdk;

Sequences::Sequences(const Model &model) {
    isNative = isConnected = isTerminal = Bytes::Constant(model.n, false);
    chainIdx = Integers::Constant(model.n, -1);
    chainBounds = std::vector<std::pair<int, int>>();

    for (int chIdx = 0; chIdx < (int)model.chains.size(); ++chIdx) {
        auto const& ch = model.chains[chIdx];
        for (int i = ch.start; i < ch.end-1; ++i) {
            isConnected[i] = 1;
            chainIdx[i] = chIdx;
        }
        isTerminal[ch.start] = isTerminal[ch.end-1] = true;
        chainBounds.emplace_back(ch.start, ch.end);

        for (auto const& curSpIdx: ch.structuredParts) {
            auto const& sp = model.structuredParts[curSpIdx];

            auto istart = ch.start + sp.off;
            auto iend = istart + sp.len;
            for (int i = istart; i < iend; ++i) {
                isNative[i] = true;
            }
        }
    }
}