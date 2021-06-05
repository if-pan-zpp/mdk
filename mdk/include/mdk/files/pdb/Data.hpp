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
        static Data onlyAtoms(pdb::Model const& model);

        pdb::Model asModel() const;

        friend Data& operator<<(Data& data, records::Record const& record);
        friend Data& operator<<(Data& data, Data const& other);
    };
}
