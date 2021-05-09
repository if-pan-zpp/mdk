#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/cpu/data/State.hpp>

namespace mdk {
    class Masses: public Scalars {
    public:
        Masses() = default;
        explicit Masses(Model const& model);
    };
}
