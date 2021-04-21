#include "io/go/LegacyParser.hpp"
#include "utils/Units.hpp"
#include <sstream>

using namespace mdk::go;
using namespace std;

StructuredPart LegacyParser::read(std::istream &is) {
    StructuredPart sp;

    int numOfContacts, numOfResidues;
    is >> sp.offset;
    is >> numOfContacts;
    is >> numOfResidues;

    sp.contacts = vector<StructuredPart::Contact>(numOfContacts);
    for (int i = 0; i < numOfContacts; ++i) {
        auto& contact = sp.contacts[i];
        is >> contact.res[0] >> contact.res[1] >> contact.dist;
        --contact.res[0];
        --contact.res[1];
        contact.dist *= f77unit;
    }

    sp.len = numOfResidues;
    sp.angle = vector<double>(numOfResidues);
    sp.dihedral = vector<double>(numOfResidues);

    for (int i = 0; i < numOfResidues; ++i) {
        is >> sp.angle[i] >> sp.dihedral[i];
        sp.angle[i] *= radian;
        sp.dihedral[i] *= radian;
    }

    return sp;
}

std::ostream &LegacyParser::write(ostream &os, StructuredPart const& sp) {
    os << sp.offset << endl;
    os << sp.contacts.size() << endl;
    os << sp.len << endl;

    for (auto& contact: sp.contacts) {
        os << contact.res[0] + 1 << "\t";
        os << contact.res[1] + 1 << "\t";
        os << contact.dist / (5.0 * angstrom) << endl;
    }

    for (size_t i = 0; i < sp.len; ++i) {
        os << sp.angle[i] / radian << "\t";
        os << sp.dihedral[i] / radian << endl;
    }

    return os;
}
