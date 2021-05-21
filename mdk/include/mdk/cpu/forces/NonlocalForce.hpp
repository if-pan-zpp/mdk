#pragma once
#include <mdk/cpu/forces/Force.hpp>
#include <mdk/cpu/verlet/Factory.hpp>
#include <mdk/cpu/verlet/Spec.hpp>

namespace mdk {
    class NonlocalForce: public Force {
    protected:
        mutable vl::Spec _spec;
        virtual vl::Spec recomputeSpec() const = 0;

    public:
        vl::Spec const& spec() const;
        virtual void onVLUpdate(Pairs& vl) = 0;
    };
}