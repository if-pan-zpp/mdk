#include "cpu/nonlocal/NativeContacts.hpp"
using namespace mdk;

NativeContacts::NativeContacts(const Model &model) {
    for (auto const& cont: model.contacts) {
        bool isDisulfide = (ContCode)cont.type == ContCode::SSBOND;

        contacts.emplace_back((NativeContact) {
            .i = cont.res[0], .j = cont.res[1],
            .dist0 = cont.dist0, .isDisulfide = isDisulfide
        });
    }
}
