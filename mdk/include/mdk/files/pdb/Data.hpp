#pragma once
#include <vector>
#include <mdk/files/pdb/Record.hpp>

namespace mdk::pdb {
    class Data {
    public:
        std::vector<records::Record> records;
    };
}