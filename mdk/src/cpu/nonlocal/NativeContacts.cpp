#include "cpu/nonlocal/NativeContacts.hpp"
using namespace mdk;

NativeContacts::NativeContacts(const Model &model,
        DisulfideV const& disulfideV) {
    this->disulfideV = disulfideV;
    for (auto const& cont: model.contacts) {
        auto isDisulfide = (ContCode)cont.type == ContCode::SSBOND;

        auto ix = std::make_pair(cont.res[0], cont.res[1]);
        if (isDisulfide) {
            disulfides.emplace_back(ix, NativeDisulfide {});
        }
        else {
            NativeNormal normal { .r_min = cont.dist0 };
            normals.emplace_back(ix, normal);
        }
    }
}

double NativeContacts::cutoff() const {
    double maxCutoff = 0.0;
    for (auto const& [ix, nn]: normals) {
        maxCutoff = std::max(maxCutoff, nn.r_min);
    }
    maxCutoff = std::max(maxCutoff, disulfideV.cutoff());

    _cutoff = maxCutoff;
    return maxCutoff;
}