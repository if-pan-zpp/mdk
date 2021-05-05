#include "cpu/local/NativeBondAngles.hpp"
using namespace mdk;

NativeBondAngles::NativeBondAngles(const Model &model) {
    theta0 = Scalars(model.n);

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