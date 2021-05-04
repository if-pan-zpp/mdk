#pragma once
#include <vector>
#include <Eigen/Geometry>
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/utils/Topology.hpp>

namespace mdk {
    class State {
    public:
        int n;
        double t;
        Vectors r, v;
        Scalars q;

        Topology top;
    };
}
