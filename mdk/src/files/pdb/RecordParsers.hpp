#pragma once
#include <memory>
#include "files/pdb/Field.hpp"
#include "files/pdb/Record.hpp"

namespace mdk {
    class RecordParser {
    protected:
        Record record;

    public:
        std::vector<std::shared_ptr<Field>> fields;

        Record tryParse(std::string const& s) const;
        std::ostream& write(std::ostream& os, Record const& other);
    };

    class AtomParser: public RecordParser {
    public:
        AtomParser();
    };

    class SSBondParser: public RecordParser {
    public:
        SSBondParser();
    };

    class Cryst1Parser: public RecordParser {
    public:
        Cryst1Parser();
    };

    class EndParser: public RecordParser {
    public:
        EndParser();
    };

    class ModelParser: public RecordParser {
    public:
        ModelParser();
    };
}