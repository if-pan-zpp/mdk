#pragma once
#include <vector>

namespace mdk::go {
    class StructuredPart {
    public:
        int offset;

        struct Contact {
            int res1, res2;
            double eqDist;
        };

        std::vector<Contact> contacts;
        std::vector<double> tether, angle, dihedral;
    };
}