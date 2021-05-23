#pragma once
#include <forces/go/GoBase.hpp>

namespace mdk {
    class NormalNative: public GoBase {
    public:
        double depth = 1.0 * eps;

        explicit NormalNative(Model const& model);
        void eval(BaseState const& state, BaseDiff& update) const override;

    protected:
        vl::Spec recomputeSpec() const override;
    };
}