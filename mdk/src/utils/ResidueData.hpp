#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "utils/AminoAcid.hpp"

namespace mdk {
    struct AtomTypeInfo {
        double radius;
        bool inBackbone = false;
    };

    struct ResidueTypeInfo {
        double mass;
        std::unordered_set<std::string> heavyAtoms;
        std::unordered_map<std::string, AtomTypeInfo> atomInfo;
    };

    using ResidueData = std::unordered_map<AminoAcid, ResidueTypeInfo>;

    ResidueData createResData();
}
