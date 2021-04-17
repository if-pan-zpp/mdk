#include "files/pdb/Parser.hpp"
#include "utils/Text.hpp"
#include "files/pdb/LineParsers.hpp"
using namespace mdk;
using namespace std;

PDBParser::PDBParser() {
    parsers = {
        { Record::ATOM, make_shared<AtomParser>(record.atom)},
        { Record::SSBOND, make_shared<SSBondParser>(record.ssbond)},
        { Record::CRYST1, make_shared<Cryst1Parser>(record.cryst1)},
        { Record::END, make_shared<EndParser>()}
    };
}

PDBFile PDBParser::read(istream &is) {
    PDBFile pdb;
    for (string line; getline(is, line); ) {
        assert(line.size() == 80+1);
        for (auto const& [tag, parser]: parsers) {
            if (parser->tryParse(line)) {
                record.tag = tag;
                break;
            }
        }
        pdb.records.push_back(record);

        if (record.tag == Record::END)
            break;
    }
    return pdb;
}

ostream &PDBParser::write(ostream &os, const PDBFile &file) {
    for (auto const& fileRecord: file.records) {
        record = fileRecord;
        os << parsers[record.tag] << endl;
    }
    return os;
}
