#include "cpu/nonlocal/NativeContacts.hpp"
using namespace mdk;

NativeContacts::NativeContacts(const Model &model) {
    for (auto const& cont: model.contacts) {
        if ((ContCode)cont.type == ContCode::SSBOND) continue;

        auto ix = std::make_pair(cont.res[0], cont.res[1]);

        NativeContact nc;
        nc.i1 = std::min(ix.first, ix.second);
        nc.i2 = std::max(ix.first, ix.second);
        nc.r_min = cont.dist0;
        normals.emplace_back(ix, std::move(nc));
    }

    std::sort(normals.begin(), normals.end(),
        [](auto const& p1, auto const& p2) -> auto {
            return p1.first < p2.first;
        });
}

double NativeContacts::cutoff() const {
    double maxCutoff = 0.0;
    for (auto const& [ix, nn]: normals) {
        maxCutoff = std::max(maxCutoff, nn.r_min);
    }

    _cutoff = maxCutoff;
    return maxCutoff;
}