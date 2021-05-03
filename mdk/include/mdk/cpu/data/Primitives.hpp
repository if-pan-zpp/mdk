#pragma once
#include <Eigen/Core>
#include <vector>

namespace mdk {
    using Scalars = Eigen::VectorXd;
    using Vector = Eigen::Vector3d;
    using Vectors = std::vector<Vector, Eigen::aligned_allocator<Vector>>;
    using Integers = std::vector<int>;
    using Regions = std::vector<std::pair<int, int>>;
    using VRef = Vector const&;
}
