#include "files/pdb/PDBParser.hpp"
#include "utils/Text.hpp"
#include "files/pdb/RecordParsers.hpp"
using namespace mdk;
using namespace std;

PDBParser::PDBParser() {
    parsers = {
        { ((Record)Atom()).index(), make_shared<AtomParser>() },
        { ((Record)SSBond()).index(), make_shared<SSBondParser>() },
        { ((Record)Cryst1()).index(), make_shared<Cryst1Parser>() },
        { ((Record)End()).index(), make_shared<EndParser>() },
        { ((Record)Model()).index(), make_shared<ModelParser>() }
    };
}

PDBFile PDBParser::read(istream &is) {
    PDBFile pdb;

    for (string line; getline(is, line); ) {
        if (line.size() < 80)
            line.insert(line.size(), 80 - line.size(), ' ');

        for (auto const& [idx, parser]: parsers) {
            auto record = parser->tryParse(line);
            if (!holds_alternative<monostate>(record)) {
                pdb.records.push_back(record);
                if (holds_alternative<End>(record)) return pdb;
                else break;
            }
        }
    }

    return pdb;
}

std::ostream &PDBParser::write(ostream &os, const PDBFile &file) {
    for (auto const& record: file.records) {
        parsers[record.index()]->write(os, record);
        os << endl;
    }
    return os;
}
