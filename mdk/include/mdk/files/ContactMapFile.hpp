#pragma once

#include <istream>
#include <ostream>
#include <vector>

namespace mdk {
    struct StructuredPart {
        int offset;

        struct Contact {
            int res1, res2;
            double eqDist;
        };
        std::vector<Contact> contacts;

        std::vector<double> tether;
        std::vector<double> angle;
        std::vector<double> dihedral;
    };

    class ContactMapFile {
    public:
        ContactMapFile(std::istream& is);

        std::ostream &operator>>(std::ostream &os);

        StructuredPart data;
    };
}