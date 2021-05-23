#pragma once
#include <vector>
#include "Record.hpp"
#include "../../model/AtomicModel.hpp"

namespace mdk::pdb {
    class Data {
    public:
        std::vector<records::Record> records;

    public:
        Data() = default;
        explicit Data(AtomicModel const& model);
        AtomicModel asModel() const;
    };
}
