#pragma once
#include <vector>
#include <Eigen/Geometry>
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/utils/Topology.hpp>
#include "Stat.hpp"

namespace mdk {
    class State {
    public:
        int n;

        double t;
        Vectors r, v;
        Topology top;
        std::vector<Stat> stats;

        State() = default;
        State(Model const& model);
    };
}
