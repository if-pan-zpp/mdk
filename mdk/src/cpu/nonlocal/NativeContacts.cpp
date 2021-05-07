#include "cpu/nonlocal/NativeContacts.hpp"
using namespace mdk;

NativeContacts::NativeContacts(const Model &model,
        DisulfideV const& disulfideV) {
    this->disulfideV = disulfideV;
    for (auto const& cont: model.contacts) {
        auto isDisulfide = (ContCode)cont.type == ContCode::SSBOND;

        double r_min;
        if (isDisulfide) {
            disulfides.emplace_back(cont.idx[0], cont.idx[1], NativeDisulfide {});
            r_min = disulfideV.dist0;
        }
        else {
            NativeNormal normal;
            normal.ff.depth = 1.0 * eps;
            normal.ff.r_min = cont.dist0;
            normals.emplace_back(cont.idx[0], cont.idx[1], normal);
            r_min = cont.dist0;
        }

        cutoff2 = std::max(cutoff2, r_min * r_min);
    }
}
