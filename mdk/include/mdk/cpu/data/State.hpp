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

        Eigen::VectorXi cysNeigh;
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> cysTaken;
        std::vector<Stat> stats;

        State() = default;
        State(Model const& model);
    };
}
