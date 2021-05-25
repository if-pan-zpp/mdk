#pragma once
#include <Eigen/Dense>
#include "Vectors.hpp"
#include "Ranges.hpp"

namespace mdk {
    using Scalars = Eigen::VectorXd;
    using Vector = Eigen::Vector3d;
    using VRef = Vector const&;

    using Bytes = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>;
    using Integers = Eigen::Matrix<int, Eigen::Dynamic, 1>;
}
