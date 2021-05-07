#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/cpu/data/State.hpp>

namespace mdk {
    class Masses {
    public:
        Scalars mass;

        Masses() = default;
        explicit Masses(Model const& model);
        Masses(Model const& model, double mass0);

        double& operator[](int idx) {
            return mass[idx];
        }

        double const& operator[](int idx) const {
            return mass[idx];
        }
    };
}
