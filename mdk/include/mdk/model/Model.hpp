#pragma once
#include <vector>
#include <Eigen/Core>
#include <string>

namespace mdk {
    class Model {
    public:
        struct Atom;
        struct Residue;

        struct Atom {
            int id;
            Eigen::Vector3d pos;
            std::string type;
        };
        std::vector<Atom> atoms;

        struct Bond {
            Atom *atom[2];
            std::string type;
        };
        std::vector<Bond> bonds;
    };
}