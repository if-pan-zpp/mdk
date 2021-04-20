#include "io/pdb/Model.hpp"
#include "utils/ResidueTypes.hpp"
using namespace mdk;
using namespace mdk::pdb;
using namespace std;

mdk::Model pdb::Model::reduce(bool contactsFromAllAtoms) {
    mdk::Model model;
}

bool pdb::Model::hasAllAtoms() const {
    for (auto const& [chainID, chain]: chains) {
        for (auto const& [resID, res]: chain.residues) {
            if (AminoAcid::isAminoAcid(res.type)) {
                if (res.atoms.size() != types[res.type].atoms.size())
                    return false;
            }
        }
    }
    return true;
}

vector<mdk::Model::Bond> pdb::Model::allAtomCContacts() {

}

vector<mdk::Model::Bond> pdb::Model::onlyCAContacts() {

}
