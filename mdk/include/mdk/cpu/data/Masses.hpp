#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class Masses: public Scalars {
    public:
        Masses() = default;
        explicit Masses(Model const& model);
    };
}
