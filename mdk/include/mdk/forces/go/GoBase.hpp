#pragma once
#include <forces/NonlocalForce.hpp>
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class GoBase: public NonlocalForce {
    public:
        struct Contact {
            int i1, i2;
            double r_min;
        };

        virtual void onVLUpdate(Pairs& vl, BaseState& state) override;

    protected:
        std::vector<Contact> allContacts, curPairs;
        virtual vl::Spec recomputeSpec() const override;

    private:
        Pairs newVl;
    };
}
