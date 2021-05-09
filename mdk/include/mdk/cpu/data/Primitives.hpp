#pragma once
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <vector>

namespace mdk {
    using Scalars = Eigen::VectorXd;
    using Vector = Eigen::Vector3d;
    using VRef = Vector const&;

    using Vectors = std::vector<Vector, Eigen::aligned_allocator<Vector>>;
    using Bytes = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>;
    using Integers = Eigen::Matrix<int, Eigen::Dynamic, 1>;
}
