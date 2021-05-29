#include "forces/angle/NativeBA.hpp"
#include "simul/Simulation.hpp"
#include "forces/angle/BondAngles.hpp"
using namespace mdk;

void NativeBA::bind(Simulation &simulation) {
    auto const& model = simulation.data<Model>();
    isNative = Bytes(model.n, false);
    theta0 = Scalars(model.n);

    for (auto const& ch: model.chains) {
        for (auto const& spIdx: ch.structuredParts) {
            auto const& sp = model.structuredParts[spIdx];
            auto spStart = ch.start + sp.off + 1;
            auto spEnd = ch.start + sp.off + sp.len - 1;

            for (int i = spStart; i < spEnd; ++i) {
                isNative[i] = true;
                theta0[i] = sp.angle[i - (ch.start + sp.off)];
            }
        }
    }

    auto& unifiedBA = simulation.var<BondAngles>();
    unifiedBA.natBA = this;
}