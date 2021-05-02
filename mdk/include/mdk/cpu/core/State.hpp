#pragma once
#include <vector>
#include <mdk/cpu/core/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class State {
    public:
        int n;
        double t;
        Vectors r, v;
        Scalars q;

    public:
        State() = default;
        State(Model const& model);
    };
}
