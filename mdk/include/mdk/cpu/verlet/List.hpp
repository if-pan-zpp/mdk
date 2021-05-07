#pragma once
#include <mdk/utils/Metavector.hpp>

namespace mdk::vl {
    struct Base {
        int i1, i2;
        Vector unit;
        double norm;

        inline bool operator<(Base const& other) const {
            auto p1 = std::make_pair(i1, i2);
            auto p2 = std::make_pair(other.i1, other.i2);
            return p1 < p2;
        }
    };

    template<typename... Xs>
    class List: public Metavector<vl::Base, Xs...> {
    public:
        virtual double cutoff2() = 0;
        virtual double refine() = 0;
    };
}
