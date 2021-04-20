#include "io/pdb/Model.hpp"
#include "utils/ResidueTypes.hpp"
using namespace mdk;
using namespace mdk::pdb;
using namespace std;

CoarseModel Model::coarsen(bool contactsFromAllAtoms) {
    CoarseModel coarseModel;
}

bool Model::hasAllAtoms() const {
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

vector<CoarseModel::Bond> Model::allAtomCContacts() {
    return std::vector<CoarseModel::Bond>();
}

vector<CoarseModel::Bond> Model::onlyCAContacts() {
    return std::vector<CoarseModel::Bond>();
}
