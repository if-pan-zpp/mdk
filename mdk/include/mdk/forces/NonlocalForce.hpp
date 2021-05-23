#pragma once
#include <forces/Force.hpp>
#include <verlet/List.hpp>
#include <verlet/Spec.hpp>

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