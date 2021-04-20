#pragma once
#include <vector>
#include <Eigen/Core>
#include <unordered_map>
#include <mdk/utils/AminoAcid.hpp>
#include <mdk/model/Model.hpp>

namespace mdk::pdb {
    class Model {
    public:
        struct Atom;
        struct Residue;
        struct Chain;

        struct Atom {
            int id;
            Residue *res;
            Eigen::Vector3d pos;
            std::string name, element;
        };

        struct Residue {
            int id;
            Chain *chain;
            std::string type;
            std::unordered_map<int, Atom> atoms;
        };

        struct Chain {
            char id;
            std::unordered_map<int, Residue> residues;
        };
        std::unordered_map<char, Chain> chains;

        struct SSBond {
            int id;
            Residue *res[2];
            double dist0;
        };
        std::unordered_map<int, SSBond> ssbonds;

        Eigen::Vector3d cellDims;

    public:
        mdk::Model reduce(bool contactsFromAllAtoms = true);

    private:
        bool hasAllAtoms() const;
        std::vector<mdk::Model::Bond> allAtomCContacts();
        std::vector<mdk::Model::Bond> onlyCAContacts();
    };
}