#pragma once
#include <Eigen/Core>
#include <Eigen/StdVector>
#include <vector>

namespace mdk {
    using Scalars = Eigen::VectorXd;
    using Vector = Eigen::Vector3d;
    using VRef = Vector const&;

    using Bytes = Eigen::Matrix<int8_t, Eigen::Dynamic, 1>;
    using Integers = Eigen::Matrix<int, Eigen::Dynamic, 1>;

    using VectorBase = Eigen::Matrix<double, 3, Eigen::Dynamic, Eigen::RowMajor>;
    class Vectors: public VectorBase {
    public:
        Vectors() = default;

        explicit Vectors(int n):
            VectorBase(3, n) {};

        Vectors(int n, Vector const& init) {
            resize(3, n);
            colwise() = init;
        }

        inline auto operator[](int i) {
            return col(i);
        }

        inline auto operator[](int i) const {
            return col(i);
        }
    };
}
