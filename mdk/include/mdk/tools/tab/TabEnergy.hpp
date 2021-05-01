#pragma once
#include <vector>
#include <mdk/utils/PairType.hpp>

namespace mdk::tab {
    class TabFunction {
    public:
        double a, b;
        std::vector<double> values;
    };

    class TabEnergy {
    public:
        std::unordered_map<PairType, TabFunction> angleV;
        std::unordered_map<PairType, TabFunction> dihedralV;
    };
}