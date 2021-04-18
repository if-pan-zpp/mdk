#include "utils/AminoAcid.hpp"
#include <algorithm>

using namespace mdk;
using namespace std;

AminoAcid::AminoAcid() {
    codeToName = {
        {'A', "ALA"},
        {'R', "ARG"},
        {'N', "ASN"},
        {'D', "ASP"},
        {'C', "CYS"},
        {'E', "GLU"},
        {'Q', "GLN"},
        {'G', "GLY"},
        {'H', "HIS"},
        {'I', "ILE"},
        {'L', "LEU"},
        {'K', "LYS"},
        {'M', "MET"},
        {'F', "PHE"},
        {'P', "PRO"},
        {'S', "SER"},
        {'T', "THR"},
        {'W', "TRP"},
        {'Y', "TYR"},
        {'V', "VAL"}
    };

    nameToCode = {
        {"ALA", 'A'},
        {"ARG", 'R'},
        {"ASN", 'N'},
        {"ASP", 'D'},
        {"CYS", 'C'},
        {"GLU", 'E'},
        {"GLN", 'Q'},
        {"GLY", 'G'},
        {"HIS", 'H'},
        {"ILE", 'I'},
        {"LEU", 'L'},
        {"LYS", 'K'},
        {"MET", 'M'},
        {"PHE", 'F'},
        {"PRO", 'P'},
        {"SER", 'S'},
        {"THR", 'T'},
        {"TRP", 'W'},
        {"TYR", 'Y'},
        {"VAL", 'V'}
    };

    numAminoAcids = 20;
    allAminoAcids = makeAminoAcids();
}

AminoAcid::AminoAcid(char c) {
    code = c;
    name = codeToName[c];
}

AminoAcid::AminoAcid(const string &s) {
    name = s;
    code = nameToCode[s];
}

AminoAcid::operator char() const {
    return code;
}

AminoAcid::operator std::string() const {
    return name;
}

bool AminoAcid::operator==(AminoAcid const &aminoAcid2) const {
    return code == aminoAcid2.code;
}

std::vector<AminoAcid> AminoAcid::makeAminoAcids() {
    std::vector<AminoAcid> aminoAcids(numAminoAcids);
    string codes = "GPQCASVTILNDKEMHFRYW";

    for (int i = 0; i < numAminoAcids; ++i) {
        aminoAcids[i] = (AminoAcid)codes[i];
    }

    return aminoAcids;
}
