#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class DihedralRanges {
    public:
        Ranges all, native, nonNative;
        explicit DihedralRanges(Model const& model);
    };
}