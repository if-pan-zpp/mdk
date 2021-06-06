#pragma once
#include <Eigen/Dense>
#include "Vectors.hpp"

namespace mdk {
    using Scalars = Eigen::VectorXd;
    using Vector = Eigen::Vector3d;
    using VRef = Vector const&;

    using Bytes = std::vector<int8_t>;
    using Integers = std::vector<int>;
    using Pairs = std::vector<std::pair<int, int>>;
}
