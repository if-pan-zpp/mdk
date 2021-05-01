#include "utils/AminoAcid.hpp"
#include <algorithm>
using namespace mdk;
using namespace std;

std::vector<AAType> AminoAcid::types() {
    return {
        AAType::ALA, AAType::ARG, AAType::ASN, AAType::ASP, AAType::CYS,
        AAType::GLU, AAType::GLN, AAType::GLY, AAType::HIS, AAType::ILE,
        AAType::LEU, AAType::LYS, AAType::MET, AAType::PHE, AAType::PRO,
        AAType::SER, AAType::THR, AAType::TRP, AAType::TYR, AAType::VAL
    };
}

std::vector<AminoAcid> AminoAcid::aminoAcids() {
    std::vector<AminoAcid> _aminoAcids;
    for (auto code: codes()) {
        _aminoAcids.emplace_back(code);
    }
    return _aminoAcids;
}

AminoAcid::operator AAType const&() const {
    return type;
}

bool AminoAcid::isProper(char code) {
    static const auto _codes = codes();
    return _codes.find(code) != string::npos;
};

AminoAcid::AminoAcid(char code) {
    static const auto _codes = codes();
    type = (AAType)_codes.find(code);
}

std::string AminoAcid::codes() {
    return "ARNDCEQGHILKMFPSTWYV";
}

AminoAcid::operator char const&() const {
    static const auto _codes = codes();
    return _codes[(int8_t)type];
}

bool AminoAcid::isProper(std::string const& name) {
    static const auto _names = names();
    return find(_names.begin(), _names.end(), name) != _names.end();
}

AminoAcid::AminoAcid(std::string const& name) {
    static const auto _names = names();
    auto iter = find(_names.begin(), _names.end(), name);
    if (iter != _names.end()) {
        static const auto _types = types();
        auto idx = distance(_names.begin(), iter);
        type = _types[idx];
    }
}

std::vector<std::string> AminoAcid::names() {
    return {
        "ALA", "ARG", "ASN", "ASP", "CYS", "GLU", "GLN", "GLY", "HIS", "ILE",
        "LEU", "LYS", "MET", "PHE", "PRO", "SER", "THR", "TRP", "TYR", "VAL"
    };
}

AminoAcid::operator string const&() const {
    static const auto _names = names();
    return _names[(int8_t)type];
}

bool AminoAcid::operator==(const AminoAcid &other) const {
    return type == other.type;
}

bool AminoAcid::operator!=(const AminoAcid &other) const {
    return type != other.type;
}

bool AminoAcid::operator<(const AminoAcid &other) const {
    return type < other.type;
}

bool AminoAcid::operator<=(const AminoAcid &other) const {
    return type <= other.type;
}

bool AminoAcid::operator>(const AminoAcid &other) const {
    return type > other.type;
}

bool AminoAcid::operator>=(const AminoAcid &other) const {
    return type >= other.type;
}
