#pragma once
#include "Force.hpp"
#include "../verlet/List.hpp"
#include "../verlet/Spec.hpp"

namespace mdk {
    class NonlocalForce: public Force {
    protected:
        mutable vl::Spec _spec;
        virtual vl::Spec spec() const = 0;

        vl::List *vl;

    public:
        void bind(Simulation& simulation) override;
    };
}