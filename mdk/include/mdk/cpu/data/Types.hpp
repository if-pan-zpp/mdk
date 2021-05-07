#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class Types {
    private:
        Eigen::Matrix<ResType, Eigen::Dynamic, 1> types;

    public:
        Types() = default;
        explicit Types(Model const& model);

        ResType& operator[](int idx) {
            return types[idx];
        }

        ResType const& operator[](int idx) const {
            return types[idx];
        }
    };
}
