#pragma once
#include <mdk/tools/tab/TabEnergy.hpp>
#include <iostream>

namespace mdk::tab {
    class LegacyParser {
    public:
        TabEnergy read(std::istream &is);
        void write(std::ostream& os, TabEnergy const& tabEnergy);
    };
}