#pragma once
#include <boost/icl/interval_set.hpp>
#include <vector>

namespace mdk {
    using Pairs = std::vector<std::pair<int, int>>;
    using Ranges = boost::icl::interval_set<int>;
}