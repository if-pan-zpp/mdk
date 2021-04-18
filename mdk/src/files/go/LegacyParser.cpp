#include "files/go/LegacyParser.hpp"
#include "utils/Units.hpp"
#include <sstream>
#include <files/param/LegacyParser.hpp>

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
        is >> contact.res1 >> contact.res2 >> contact.eqDist;
        --contact.res1;
        --contact.res2;
        contact.eqDist *= 5.0 * angstrom;
    }

    sp.tether = vector<double>(numOfResidues, 0);
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
    os << sp.tether.size() << endl;

    for (auto& contact: sp.contacts) {
        os << contact.res1 + 1 << "\t";
        os << contact.res2 + 1 << "\t";
        os << contact.eqDist / (5.0 * angstrom) << endl;
    }

    for (size_t i = 0; i < sp.tether.size(); ++i) {
        os << sp.angle[i] / radian << "\t";
        os << sp.dihedral[i] / radian << endl;
    }

    return os;
}
