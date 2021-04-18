#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "mdk/files/pdb/Record.hpp"

namespace mdk {
    class PDBFile {
    public:
        std::vector<Record> records;
    };

    class RecordParser;

    class PDBParser {
    private:
        std::unordered_map<int, std::shared_ptr<RecordParser>> parsers;

    public:
        PDBParser();

        PDBFile read(std::istream& is);
        std::ostream& write(std::ostream& os, PDBFile const& file);
    };
}