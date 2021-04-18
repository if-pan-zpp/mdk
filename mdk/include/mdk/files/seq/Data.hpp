#pragma once
#include "utils/AminoAcid.hpp"
#include "files/go/StructuredPart.hpp"

namespace mdk::seq {
    class Data {
    public:
        struct Chain {
            std::vector<AminoAcid> codes;
            std::vector<std::string> structuredParts;
        };

        std::optional<double> screend;
        std::vector<Chain> chains;
        std::unordered_map<std::string, go::StructuredPart> structuredParts;
    };
}