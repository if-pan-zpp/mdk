#pragma once
#include <iostream>
#include "mdk/files/go/StructuredPart.hpp"

namespace mdk::go {
    class LegacyParser {
    public:
        StructuredPart read(std::istream& is);
        std::ostream& write(std::ostream& os, StructuredPart const& sp);
    };
}