#pragma once
#include <forces/NonlocalForce.hpp>
#include <kernels/ShiftedTruncatedLJ.hpp>

namespace mdk {
    class PauliExclusion: public NonlocalForce {
    public:
        ShiftedTruncatedLJ stlj;

        void reconstructFrom(Pairs& vl) override;
        void eval(BaseState const& state, BaseDiff& update) const override;

    protected:
        vl::Spec recomputeSpec() const override;

    private:
        Pairs exclPairs;
    };
}