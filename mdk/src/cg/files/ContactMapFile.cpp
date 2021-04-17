#include "files/ContactMapFile.hpp"
#include "utils/Units.hpp"
#include <sstream>
using namespace mdk;
using namespace std;

template<typename... Types>
void fetchFromLine(std::istream& is, Types&... args) {
    stringstream sstream;
    string line; getline(is, line);
    sstream << line;
    (sstream >> ... >> args);
}

ContactMapFile::ContactMapFile(std::istream &is) {
    int numOfContacts, numOfResidues;
    fetchFromLine(is, data.offset);
    fetchFromLine(is, numOfContacts);
    fetchFromLine(is, numOfResidues);

    data.contacts = vector<StructuredPart::Contact>(numOfContacts);
    for (int i = 0; i < numOfContacts; ++i) {
        auto& contact = data.contacts[i];
        fetchFromLine(is, contact.res1, contact.res2, contact.eqDist);
        --contact.res1;
        --contact.res2;
        contact.eqDist *= 5.0 * angstrom;
    }

    data.tether = vector<double>(numOfResidues, 0);
    data.angle = vector<double>(numOfResidues);
    data.dihedral = vector<double>(numOfResidues);

    for (int i = 0; i < numOfResidues; ++i) {
        fetchFromLine(is, data.angle[i], data.dihedral[i]);
        data.angle[i] *= radian;
        data.dihedral[i] *= radian;
    }
}

std::ostream &ContactMapFile::operator>>(ostream &os) {
    os << data.offset << endl;
    os << data.contacts.size() << endl;
    os << data.tether.size() << endl;

    for (auto& contact: data.contacts) {
        os << contact.res1 + 1 << "\t";
        os << contact.res2 + 1 << "\t";
        os << contact.eqDist / (5.0 * angstrom) << endl;
    }

    for (int i = 0; i < data.tether.size(); ++i) {
        os << data.angle[i] / radian << "\t";
        os << data.dihedral[i] / radian << endl;
    }

    return os;
}

