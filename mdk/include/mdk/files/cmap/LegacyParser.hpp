#pragma once
#include <iostream>
#include "ContactMap.hpp"

namespace mdk::cmap {
    class LegacyParser {
    public:
        ContactMap read(std::istream& is);
        std::ostream& write(std::ostream& os, ContactMap const& cmap);
    };
}
