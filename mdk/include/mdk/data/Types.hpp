#pragma once
#include "Primitives.hpp"
#include "../model/Model.hpp"

namespace mdk {
    class Types: public Eigen::Matrix<ResType, Eigen::Dynamic, 1> {
    public:
        Types() = default;
        explicit Types(Model const& model);
    };
}
