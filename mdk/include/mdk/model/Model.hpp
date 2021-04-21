#pragma once
#include <vector>
#include <Eigen/Core>
#include <string>
#include <mdk/utils/Units.hpp>

namespace mdk {
    class Model {
    public:
        struct Atom;
        struct Chain;

        struct Atom {
            int idx;
            Chain *chain;
            Eigen::Vector3d pos;
            std::vector<std::string> types;
        };

        struct StructuredPart {
            int len, offset;
            std::vector<double> angle, dihedral;
        };

        struct Chain {
            int idx;
            std::vector<Atom> atoms;
            std::vector<double> tethers;
            std::vector<StructuredPart> structuredParts;
        };
        std::vector<Chain> chains;

        struct Contact {
            Atom *atom[2];
            std::string type;
            double dist;
        };
        std::vector<Contact> contacts;

    public:
        void morphIntoLine();
        void morphIntoSAW();
    };
}