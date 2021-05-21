#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class BondAngleRanges {
    public:
        Ranges all, native, nonNative;
        explicit BondAngleRanges(Model const& model);
    };
}