#pragma once
#include <eigen3/Eigen/Core>
#include <string>
#include <variant>

namespace mdk::pdb {
    class Atom {
    public:
        int serialNum;
        std::string atomName;
        char altLocation;
        std::string residueName;
        char chainID;
        int residueSeqNum = 0;
        char insertionCode;
        Eigen::Vector3d pos;
        double occupancy;
        double tempFactor;
        std::string element;
        std::string charge;
    };

    class SSBond {
    public:
        int serialNum;

        struct PerResidueData {
            char chainId;
            int residueSeqNum;
            char insertionCode;
            std::string symmetryOp;
        };
        PerResidueData res[2];

        double eqDist;
    };

    class Cryst1 {
    public:
        Eigen::Vector3d size;
        Eigen::Vector3d angles;
        std::string spaceGroup;
        int z;
    };

    class Model {
    public:
        int serialNum;
    };

    class End {
    public:
    };

    using Record = std::variant<std::monostate, Atom, SSBond, Cryst1, End, Model>;
}