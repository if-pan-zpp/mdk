#pragma once
#include <mdk/cpu/forces/es/ESBase.hpp>

namespace mdk {
    class RelativeDH: public ESBase {
    protected:
        vl::Spec recomputeSpec() const override;

    public:
        double screeningDist = 10.0 * angstrom;
        double r0 = 4.0 * angstrom;

        RelativeDH(Model const& model, param::Parameters const& params);

        void eval(BaseState const& state, BaseDiff& update) const override;
    };
}