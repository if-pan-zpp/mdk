#pragma once
#include <vector>
#include "Record.hpp"
#include "Model.hpp"

namespace mdk::pdb {
    class Data {
    public:
        std::vector<records::Record> records;

    public:
        Data() = default;
        explicit Data(pdb::Model const& model);
        db::Model asModel() const;
    };
}
