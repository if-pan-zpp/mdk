#pragma once
#include <Eigen/Core>
#include <vector>

namespace mdk {
    using Scalars = Eigen::VectorXd;
    using Vector = Eigen::Vector3d;
    using Vectors = std::vector<Vector, Eigen::aligned_allocator<Vector>>;
    using VRef = Vector const&;
    using Ranges = std::vector<std::pair<int, int>>;
}
