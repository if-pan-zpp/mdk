#pragma once
#include <eigen3/Eigen/Core>
#include <string>

namespace mdk {
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

    class End {
    public:
    };

    struct Record {
    public:
        Record();
        ~Record();
        Record(Record const& other);
        Record(Record&& other) noexcept;
        Record& operator=(Record const& other);

        enum Types { ATOM, SSBOND, CRYST1, END } tag;
        union { Atom atom; SSBond ssbond; Cryst1 cryst1; End end; };
    };
}