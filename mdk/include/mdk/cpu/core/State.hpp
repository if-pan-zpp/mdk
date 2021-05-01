#pragma once
#include <vector>
#include <mdk/cpu/core/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>

namespace mdk {
    class State {
    public:
        int n;
        double t;
        Vectors r, v;
        Scalars q, m, m_inv;

        Integers chainIdx;
        Regions chains;

        enum Types {
            ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE,
            LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL
        };
        List<int8_t> types;

    public:
        State() = default;
        State(Model const& model);
    };
}
