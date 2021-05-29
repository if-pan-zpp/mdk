#pragma once
#include "ESBase.hpp"

namespace mdk {
    class ConstDH: public ESBase {
    protected:
        vl::Spec spec() const override;

    public:
        double screeningDist = 10.0 * angstrom;
        double permittivity = 80.0 * epsilon_0;

        void asyncPart(Dynamics &) override;
    };
}
