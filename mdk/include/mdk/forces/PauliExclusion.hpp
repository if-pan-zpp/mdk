#pragma once
#include "NonlocalForce.hpp"
#include "../kernels/ShiftedTruncatedLJ.hpp"
#include "../data/Chains.hpp"

namespace mdk {
    class PauliExclusion: public NonlocalForce {
    public:
        ShiftedTruncatedLJ stlj;

        PauliExclusion();
        void bind(Simulation& simulation) override;
        void asyncPart() override;
        void vlUpdateHook() override;

    protected:
        vl::Spec spec() const override;

    private:
        Pairs exclPairs;
	Chains const* chains;
    };
}
