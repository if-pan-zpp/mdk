#pragma once
#include <mdk/model/Model.hpp>
#include "mdk/utils/AminoAcid.hpp"
#include <mdk/io/go/StructuredPart.hpp>

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

    public:
        Model asModel(double dist0 = 3.8*angstrom) const;
    };
}