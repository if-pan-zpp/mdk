#include <cpu/forces/angle/NativeBondAngle.hpp>
#include <cpu/data/BondAngleRanges.hpp>
using namespace mdk;

NativeBondAngle::NativeBondAngle(const Model &model) {
    theta0 = Scalars(model.n);
    ranges = BondAngleRanges(model).native;

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

void NativeBondAngle::bondAngleTerm(int i, double theta, double &V,
        double &dV_dth) const  {

    auto diff = theta - theta0[i];
    V += CBA * diff * diff;
    dV_dth += 2.0 * CBA * diff;
}