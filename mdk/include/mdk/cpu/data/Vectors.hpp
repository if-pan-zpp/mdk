#pragma once
#include <Eigen/Core>

namespace mdk {
    using Vector = Eigen::Vector3d;
    using VRef = Vector const&;

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

        int size() {
            return cols();
        }

        inline auto operator[](int i) {
            return col(i);
        }

        inline auto operator[](int i) const {
            return col(i);
        }
    };
}