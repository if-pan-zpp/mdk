#pragma once
#include "Primitives.hpp"
#include "../files/param/Parameters.hpp"
#include "../model/Model.hpp"

namespace mdk {
    class Charges: public Scalars {
    public:
        Charges() = default;
        Charges(Model const& model, param::Parameters const& params);
    };
}