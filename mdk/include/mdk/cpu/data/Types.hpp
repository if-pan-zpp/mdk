#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class Types: public Eigen::Matrix<ResType, Eigen::Dynamic, 1> {
    public:
        Types() = default;
        explicit Types(Model const& model);
    };
}
