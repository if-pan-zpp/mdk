#pragma once
#include "Primitives.hpp"
#include "../model/Model.hpp"

namespace mdk {
    class Masses: public Scalars {
    public:
        Masses() = default;
        explicit Masses(Model const& model, bool mean = false);
    };
}
