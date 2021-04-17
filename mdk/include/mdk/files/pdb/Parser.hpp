#pragma once
#include <memory>
#include <unordered_map>
#include "files/pdb/LineParsers.hpp"
#include "files/pdb/Records.hpp"

namespace mdk {
    class PDBFile {
    public:
        std::vector<Record> records;
    };

    class PDBParser {
    private:
        std::unordered_map<Record::Types, std::shared_ptr<LineParser>> parsers;
        Record record;

    public:
        PDBParser();

        PDBFile read(std::istream& is);
        std::ostream& write(std::ostream& os, PDBFile const& file);
    };
}