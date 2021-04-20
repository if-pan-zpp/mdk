#pragma once
#include <vector>
#include <Eigen/Core>

namespace mdk {
    class CoarseModel {
    public:
        struct Residue;
        struct Chain;

        struct Bead {
            int id;
            Chain *chain;
            Eigen::Vector3d pos;
            std::string type;
        };

        struct Chain {
            int id;
            std::vector<Bead> beads;
        };
        std::vector<Chain> chains;

        struct Bond {
            int id;
            Residue *res[2];
            double dist0;
            std::string type;
        };
        std::vector<Bond> bonds;

        struct Angle {
            Residue *res[3];
            double theta0;
        };
        std::vector<Angle> angles;

        struct Dihedral {
            Residue *res[4];
            double phi0;
        };
        std::vector<Dihedral> dihedrals;
    };
}