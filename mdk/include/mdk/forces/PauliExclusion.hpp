#pragma once
#include <forces/NonlocalForce.hpp>
#include <kernels/ShiftedTruncatedLJ.hpp>

namespace mdk {
    class PauliExclusion: public NonlocalForce {
    public:
        ShiftedTruncatedLJ stlj;

        void bind(Simulation& simulation) override;
        void run() override;

    protected:
        vl::Spec spec() const override;
        void updateLocalVL() override;

    private:
        Pairs exclPairs;
    };
}