#pragma once
#include <mdk/cpu/data/Primitives.hpp>

namespace mdk {
    class Topology {
    public:
        Vector cell, cellInv;
        bool use[3];

        void setCell(VRef cell);

        inline void fix(Vector& v) const {
            for (int i = 0; i < 3; ++i) {
                if (use[i])
                    v[i] -= round(v[i] * cellInv[i]) * cell[i];
            }
        }

        inline Vector operator()(Vector v) const {
            fix(v);
            return v;
        }
    };
}
