#pragma once
#include "../../utils/AminoAcid.hpp"
#include "../cmap/ContactMap.hpp"
#include "../../model/Model.hpp"

namespace mdk::seq {
    class Sequence {
    public:
        struct Chain {
            std::vector<AminoAcid> codes;
            std::vector<std::string> contactMaps;
        };

        std::optional<double> screeningDist;
        std::vector<Chain> chains;
        std::unordered_map<std::string, cmap::ContactMap> contactMaps;

    public:
        Model asModel() const;
    };
}