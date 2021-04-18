#pragma once
#include <iostream>
#include "mdk/files/param/Data.hpp"

namespace mdk::param {
    class LegacyParser {
    private:
        void fetchDefAngleParams(std::istream& is, Data& data);
        void fetchAngleParams(std::istream& is, Data& data);
        void fetchDihedralParams(std::istream& is, Data& data);
        void fetchSpecificity(std::istream& is, Data& data);
        void fetchRadii(std::istream& is, Data& data);
        void fetchPairwiseData(std::istream& is, Data& data);

    public:
        Data read(std::istream& is);
        std::ostream& write(std::ostream& os, Data const& data);
    };
}