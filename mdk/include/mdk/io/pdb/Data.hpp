#pragma once
#include <vector>
#include <mdk/io/pdb/Record.hpp>
#include <mdk/io/pdb/Model.hpp>

namespace mdk::pdb {
    class Data {
    public:
        std::vector<records::Record> records;

    public:
        Data() = default;
        explicit Data(Model const& model);
        Model asModel() const;
    };
}