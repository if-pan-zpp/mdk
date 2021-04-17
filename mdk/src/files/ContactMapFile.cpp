#include "files/ContactMapFile.hpp"
#include "utils/Units.hpp"
#include "../utils/Text.hpp"
#include <sstream>
using namespace mdk;
using namespace std;

ContactMapFile::ContactMapFile(std::istream &is) {
    int numOfContacts, numOfResidues;
    is >> offset;
    is >> numOfContacts;
    is >> numOfResidues;

    contacts = vector<Contact>(numOfContacts);
    for (int i = 0; i < numOfContacts; ++i) {
        auto& contact = contacts[i];
        is >> contact.res1 >> contact.res2 >> contact.eqDist;
        --contact.res1;
        --contact.res2;
        contact.eqDist *= 5.0 * angstrom;
    }

    tether = vector<double>(numOfResidues, 0);
    angle = vector<double>(numOfResidues);
    dihedral = vector<double>(numOfResidues);

    for (int i = 0; i < numOfResidues; ++i) {
        is >> angle[i] >> dihedral[i];
        angle[i] *= radian;
        dihedral[i] *= radian;
    }
}

std::ostream &operator<<(ostream &os, ContactMapFile const& cmf) {
    os << cmf.offset << endl;
    os << cmf.contacts.size() << endl;
    os << cmf.tether.size() << endl;

    for (auto& contact: cmf.contacts) {
        os << contact.res1 + 1 << "\t";
        os << contact.res2 + 1 << "\t";
        os << contact.eqDist / (5.0 * angstrom) << endl;
    }

    for (size_t i = 0; i < cmf.tether.size(); ++i) {
        os << cmf.angle[i] / radian << "\t";
        os << cmf.dihedral[i] / radian << endl;
    }

    return os;
}

