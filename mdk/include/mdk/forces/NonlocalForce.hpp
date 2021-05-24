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

    public:
        std::vector<Target> req() const override;
        void bind(Simulation& simulation) override;
    };
}