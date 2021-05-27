#pragma once
#include "Force.hpp"
#include "../verlet/List.hpp"
#include "../verlet/Spec.hpp"

namespace mdk {
    class NonlocalForce: public Force {
    protected:
        mutable vl::Spec savedSpec;
        virtual vl::Spec spec() const = 0;
        vl::List *vl = nullptr;

        void installIntoVL();

    public:
        void bind(Simulation& simulation) override;
        virtual void vlUpdateHook() = 0;
    };
}