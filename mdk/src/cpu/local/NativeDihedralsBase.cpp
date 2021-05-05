#include "cpu/local/NativeDihedralsBase.hpp"
using namespace mdk;

NativeDihedralsBase::NativeDihedralsBase(const Model &model) {
    phi0 = Scalars(model.n);

    for (auto const& ch: model.chains) {
        for (auto const& spIdx: ch.structuredParts) {
            auto const& sp = model.structuredParts[spIdx];

            auto istart = ch.start + sp.off;
            auto iend = istart + sp.len;
            for (int i = istart; i < iend; ++i) {
                phi0[i] = sp.dihedral[i - istart];
            }
        }
    }
}