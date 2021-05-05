#pragma once
#include <vector>
#include <Eigen/Geometry>
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/utils/Topology.hpp>
#include <cpu/qa/Stats.hpp>

namespace mdk {
    class State {
    public:
        int n;
        double t;
        Vectors r, v;
        Topology top;
        qa::Stats qaStats;

        State() = default;
        State(Model const& model);
    };
}
