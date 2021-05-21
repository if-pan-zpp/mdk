#pragma once
#include <mdk/cpu/forces/NonlocalForce.hpp>
#include <mdk/cpu/kernels/ShiftedTruncatedLJ.hpp>

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