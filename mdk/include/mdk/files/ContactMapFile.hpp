#pragma once

#include <istream>
#include <ostream>
#include <vector>

namespace mdk {
    class ContactMapFile {
    public:
        ContactMapFile(std::istream& is);
        friend std::ostream &operator<<(std::ostream &os, ContactMapFile const& cmf);

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