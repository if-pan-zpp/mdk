#pragma once
#include "Primitives.hpp"
#include "../model/Model.hpp"

namespace mdk {
    class DihedralRanges {
    public:
        Ranges all, native, nonNative;
        Scalars phi_0;
        explicit DihedralRanges(Model const& model);
    };
}