#include "utils/AminoAcid.hpp"
#include <algorithm>

using namespace mdk;
using namespace std;

std::unordered_map<char, std::string> AminoAcid::codeToName = {
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

std::unordered_map<std::string, char> AminoAcid::nameToCode = {
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

static std::vector<AminoAcid> makeAllAminoAcids() {
    std::vector<AminoAcid> aminoAcids(AminoAcid::numAminoAcids);
    string codes = "GPQCASVTILNDKEMHFRYW";

    for (int i = 0; i < AminoAcid::numAminoAcids; ++i) {
        aminoAcids[i] = (AminoAcid)codes[i];
    }

    return aminoAcids;
}


std::vector<AminoAcid> AminoAcid::allAminoAcids = makeAllAminoAcids();

AminoAcid::AminoAcid() {
    code = 'A';
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


