#pragma once
#include <vector>
#include <string>
#include <Eigen/Core>
#include <optional>
#include "../../model/Model.hpp"
#include "../param/Parameters.hpp"

namespace mdk::pdb {
    class Model {
    public:
        struct Atom;
        struct Residue;
        struct Chain;

        struct Atom {
            int serial, idxInRes;
            Residue *res;
            Eigen::Vector3d r;
            std::string type;
        };
        std::unordered_map<int, Atom> atoms;

        Atom& addAtom(int idx, Residue *res = nullptr);

        struct Residue {
            int serial, idxInChain;
            Chain *chain;
            std::string type;
            std::vector<Atom*> atoms;

            Atom* find(std::string const& type);
        };
        std::unordered_map<int, Residue> residues;

        Residue& addResidue(int idx, Chain *chain = nullptr);

        struct Chain {
            char serial;
            std::vector<Residue*> residues;
        };
        std::unordered_map<char, Chain> chains;

        Chain& addChain(char idx);

        struct Contact {
            int idx;
            Atom* atom[2];
            std::string type;
            double dist0;
        };
        std::vector<Contact> contacts;

        Contact& addContact();

        Topology top;

    public:
        Model() = default;
        explicit Model(mdk::Model const& coarse);

        void addContactsFromAtomOverlap();
        void addContactsFromResOverlap(param::Parameters const& params);

        mdk::Model coarsen();
    };
}