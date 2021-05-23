#pragma once
#include <forces/es/ESBase.hpp>

namespace mdk {
    class ConstDH: public ESBase {
    protected:
        vl::Spec recomputeSpec() const override;

    public:
        double screeningDist = 10.0 * angstrom;
        double permittivity = 80.0 * epsilon_0;

        ConstDH(Model const& model, param::Parameters const& params);

        void eval(BaseState const& state, BaseDiff& update) const override;
    };
}