#pragma once
#include <vector>

namespace mdk::go {
    class StructuredPart {
    public:
        int len, offset;

        struct Contact {
            int res[2];
            double dist;
        };

        std::vector<Contact> contacts;
        std::vector<double> angle, dihedral;
    };
}