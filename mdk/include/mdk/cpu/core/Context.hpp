#pragma once
#include <mdk/cpu/core/Primitives.hpp>

namespace mdk {
    class Context {
    public:
        enum Types {
            ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE,
            LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL
        };
        Array<int> types;

        Array<int> chainIdx;
        List<Pair> chains;


    };
}