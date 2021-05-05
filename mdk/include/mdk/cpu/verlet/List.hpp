#pragma once
#include <vector>

namespace mdk::vl {
    template<typename Data>
    struct Pair {
        int i1, i2;
        Data data;

        inline bool operator<(Pair<Data> const& other) const {
            return std::make_pair(i1, i2) < std::make_pair(other.i1, other.i2);
        }
    };

    template<typename Data>
    class List {
    public:
        std::vector<vl::Pair<Data>> pairs;
        double cutoff2;

        virtual double finish() = 0;
    };
}
