#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "RawData.hpp"

namespace mdk::pdb {
    class RecordParser;

    class Parser {
    private:
        std::unordered_map<int, std::shared_ptr<RecordParser>> parsers;

    public:
        Parser();

        RawData read(std::istream& is);
        std::ostream& write(std::ostream& os, RawData const& data);
    };
}