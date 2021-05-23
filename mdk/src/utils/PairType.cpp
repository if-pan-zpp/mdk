#include "../../include/utils/PairType.hpp"
using namespace mdk;
using namespace std;

vector<PairType> mdk::pairTypes() {
    return {
        PairType::GG, PairType::GP, PairType::GX,
        PairType::PG, PairType::PP, PairType::PX,
        PairType::XG, PairType::XP, PairType::XX
    };
}

static int8_t encode(AminoAcid const& acid) {
    if ((AAType)acid == AAType::GLY) return 0;
    else if ((AAType)acid == AAType::PRO) return 1;
    else return 2;
}

PairType mdk::pairType(const AminoAcid &acid1, const AminoAcid &acid2) {
    return (PairType)(3*encode(acid1)+encode(acid2));
}