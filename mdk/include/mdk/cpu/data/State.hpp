#pragma once
#include <vector>
#include <cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>
#include <utils/Topology.hpp>

namespace mdk {
    class State {
    public:
        int n;
        double t;
        Vectors r, v;
        Scalars q;

        Topology top;

    public:
        State() = default;
        State(Model const& model);
    };
}
