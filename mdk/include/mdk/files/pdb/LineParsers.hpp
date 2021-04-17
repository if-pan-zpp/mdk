#pragma once
#include <memory>
#include "files/pdb/Field.hpp"
#include "files/pdb/Records.hpp"

namespace mdk {
    class LineParser {
    public:
        std::vector<std::shared_ptr<Field>> fields;

        bool tryParse(std::string const& s) const;
        friend std::ostream& operator<<(std::ostream& os, LineParser const& p);
    };

    class AtomParser: public LineParser {
    public:
        AtomParser(Atom& atom);
    };

    class SSBondParser: public LineParser {
    public:
        SSBondParser(SSBond& ssbond);
    };

    class Cryst1Parser: public LineParser {
    public:
        Cryst1Parser(Cryst1& cryst1);
    };

    class EndParser: public LineParser {
    public:
        EndParser();
    };
}