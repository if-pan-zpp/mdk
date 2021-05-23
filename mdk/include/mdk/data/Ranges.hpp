#pragma once
#include <boost/icl/interval_set.hpp>
#include <vector>

namespace mdk {
    using Pairs = std::vector<std::pair<int, int>>;

    class Ranges: public boost::icl::interval_set<int> {
    public:
        std::vector<Ranges> split(int n);
    };
}