#pragma once
#include "NonlocalForce.hpp"
#include "../kernels/ShiftedTruncatedLJ.hpp"

namespace mdk {
    class PauliExclusion: public NonlocalForce {
    public:
        ShiftedTruncatedLJ stlj;

        void bind(Simulation& simulation) override;
        void computeForce() override;

    protected:
        vl::Spec spec() const override;
        void vlUpdateHook() override;

    private:
        Pairs exclPairs;
    };
}