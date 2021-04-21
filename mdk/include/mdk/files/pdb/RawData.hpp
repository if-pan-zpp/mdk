#pragma once
#include <vector>
#include <mdk/files/pdb/Record.hpp>

namespace mdk::pdb {
    class RawData {
    public:
        std::vector<records::Record> records;
    };
}