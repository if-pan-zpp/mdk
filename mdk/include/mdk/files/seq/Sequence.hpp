#pragma once
#include "mdk/utils/AminoAcid.hpp"
#include <mdk/files/go/StructuredPart.hpp>

namespace mdk::seq {
    class Sequence {
    public:
        struct Chain {
            std::vector<AminoAcid> codes;
            std::vector<std::string> structuredParts;
        };

        std::optional<double> screeningDist;
        std::vector<Chain> chains;
        std::unordered_map<std::string, go::StructuredPart> structuredParts;
    };
}