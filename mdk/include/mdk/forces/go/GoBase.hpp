#pragma once
#include "../NonlocalForce.hpp"

namespace mdk {
    class GoBase: public NonlocalForce {
    public:
        struct Contact {
            int i1, i2;
            double r_min;
        };

        void bind(Simulation& simulation) override;

    protected:
        std::vector<Contact> allContacts, curPairs;
        vl::Spec spec() const override;

    private:
        Pairs newVl;
    };
}
