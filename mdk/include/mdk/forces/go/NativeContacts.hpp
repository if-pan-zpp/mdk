#pragma once
#include "../NonlocalForce.hpp"

namespace mdk {
    class NativeContacts: public NonlocalForce {
    public:
        struct Contact {
            int i1, i2;
            double r_min;
        };

        void bind(Simulation &simulation) override;
        void computeForce() override;

    private:
        vl::Spec spec() const override;
        void vlUpdateHook() override;

        Pairs newVL;
        std::vector<Contact> allContacts, curPairs;

        double depth = 1.0 * eps;
    };
}