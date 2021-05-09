#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class Charges: public Scalars {
    public:
        Charges() = default;
        Charges(Model const& model, param::Parameters const& params);
    };
}