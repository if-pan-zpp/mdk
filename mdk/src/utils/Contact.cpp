#include "utils/ContactType.hpp"
#include <unordered_map>
using namespace mdk;
using namespace std;

ContactType::operator ContCode const&() const {
    return code;
}

ContactType::ContactType(const std::string &name) {
    static unordered_map<string, ContCode> conversions = {
        { "NAT", ContCode::NAT },
        { "NAT_BB", ContCode::NAT_BB },
        { "NAT_BS", ContCode::NAT_BS },
        { "NAT_SB", ContCode::NAT_SB },
        { "NAT_SS", ContCode::NAT_SS },
        { "SSBOND", ContCode::SSBOND }
    };
    code = conversions.at(name);
}

ContactType::operator int8_t() const {
    return (int8_t)code;
}

ContactType::operator std::string() const {
    static unordered_map<ContCode, string> conversions = {
        { ContCode::NAT, "NAT" },
        { ContCode::NAT_BB, "NAT_BB" },
        { ContCode::NAT_BS, "NAT_BS" },
        { ContCode::NAT_SB, "NAT_SB" },
        { ContCode::NAT_SS, "NAT_SS" },
        { ContCode::SSBOND, "SSBOND" }
    };
    return conversions.at(code);
}
