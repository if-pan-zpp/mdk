#pragma once
#include "Primitives.hpp"
#include "../model/Model.hpp"

namespace mdk {
    class BondAngleRanges {
    public:
        Ranges all, native, nonNative;
        Scalars theta_0;
        explicit BondAngleRanges(Model const& model);
    };
}