#pragma once
#include <memory>
#include "Field.hpp"
#include "files/pdb/Record.hpp"

namespace mdk::pdb {
    using namespace records;

    class RecordParser {
    protected:
        Record record;

    public:
        std::vector<std::shared_ptr<Field>> fields;

        Record tryParse(std::string const& s) const;
        std::ostream& write(std::ostream& os, Record const& other);
    };

    class RemarkParser: public RecordParser {
    public:
        RemarkParser();
    };

    class AtomParser: public RecordParser {
    public:
        AtomParser();
    };

    class HetatmParser: public RecordParser {
    public:
        HetatmParser();
    };

    class SSBondParser: public RecordParser {
    public:
        SSBondParser();
    };

    class Cryst1Parser: public RecordParser {
    public:
        Cryst1Parser();
    };

    class LinkParser: public RecordParser {
    public:
        LinkParser();
    };

    class ModelParser: public RecordParser {
    public:
        ModelParser();
    };

    class EndmdlParser: public RecordParser {
    public:
        EndmdlParser();
    };

    class EndParser: public RecordParser {
    public:
        EndParser();
    };

    class TerParser: public RecordParser {
    public:
        TerParser();
    };
}