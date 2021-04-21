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
            int idx;
            Residue *res;
            Eigen::Vector3d pos;
            std::string name, element;
        };

        struct Residue {
            int idx;
            Chain *chain;
            std::string type;
            std::unordered_map<int, Atom> atoms;
        };

        struct Chain {
            char idx;
            std::unordered_map<int, Residue> residues;
        };
        std::unordered_map<char, Chain> chains;

        struct Contact {
            int idx;
            Atom *atom[2];
            double dist0;
            std::string type;
        };
        std::unordered_map<int, Contact> contacts;

        Eigen::Vector3d cell;

    public:
        mdk::Model reduce();
        void createContacts(bool fromAllAtoms = true);

    private:
        bool hasAllAtoms() const;
        std::vector<Atom*> allAtoms();
        std::vector<Atom*> contactAtoms(bool fromAllAtoms);
    };
}