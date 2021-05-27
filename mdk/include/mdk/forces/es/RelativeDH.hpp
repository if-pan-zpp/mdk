#pragma once
#include "ESBase.hpp"

namespace mdk {
    class RelativeDH: public ESBase {
    protected:
        vl::Spec spec() const override;

    public:
        double screeningDist = 10.0 * angstrom;
        double r0 = 4.0 * angstrom;

        void asyncPart() override;
    };
}