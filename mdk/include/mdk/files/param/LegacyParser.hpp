#pragma once
#include <iostream>
#include "mdk/files/param/Data.hpp"

namespace mdk::param {
    class LegacyParser {
    public:
        Data read(std::istream& is);
        std::ostream& write(std::ostream& os, Data const& data);
    };
}