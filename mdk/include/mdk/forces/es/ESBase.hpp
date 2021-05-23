#pragma once
#include <forces/NonlocalForce.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>

namespace mdk {
    class ESBase: public NonlocalForce {
    protected:
        struct Contact {
            int i1, i2, q1_q2;
        };
        std::vector<Contact> pairs;
        Integers charge;

    public:
        ESBase(Model const& model, param::Parameters const& params);
        virtual void onVLUpdate(Pairs& vl, BaseState& state) override;
    };
}
