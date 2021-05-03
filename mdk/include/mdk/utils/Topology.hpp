#pragma once
#include <mdk/cpu/data/Primitives.hpp>

namespace mdk {
    class Topology {
    public:
        Vector cell, cellInv;
        void setCell(VRef cell);

        inline void fix(Vector& v) const {
            for (int i = 0; i < 3; ++i) {
                if (cell[i] != 0.0) {
                    v[i] -= round(v[i] * cellInv[i]) * cell[i];
                }
            }
        }

        inline Vector operator()(Vector v) const {
            fix(v);
            return v;
        }
    };
}
