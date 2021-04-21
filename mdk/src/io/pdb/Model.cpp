#include "io/pdb/Model.hpp"
#include "utils/ResidueTypes.hpp"
#include "utils/Math.hpp"
using namespace mdk::pdb;
using namespace std;

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

vector<Model::Atom*> Model::allAtoms() {
    vector<Model::Atom*> atoms;
    for (auto& [chainIdx, chain]: chains) {
        for (auto& [resIdx, res]: chain.residues) {
            for (auto& [atomIdx, atom]: res.atoms) {
                atoms.push_back(&atom);
            }
        }
    }
    return atoms;
}

std::vector<Model::Atom*> Model::contactAtoms(bool fromAllAtoms) {
    vector<Model::Atom*> atoms;
    for (auto* atom: allAtoms()) {
        if (!fromAllAtoms && atom->name != "CA")
            continue;
        if (!AminoAcid::isAminoAcid(atom->res->type))
            continue;
        atoms.push_back(atom);
    }
    return atoms;
}

void Model::createContacts(bool fromAllAtoms) {
    fromAllAtoms |= hasAllAtoms();
    auto atoms = contactAtoms(fromAllAtoms);

    for (auto* atom1: atoms) {
        auto& info1 = types[atom1->res->type].atomInfo[atom1->name];
        string contactType1 = info1.inBackbone ? "B" : "S";

        for (auto* atom2: atoms) {
            auto& info2 = types[atom2->res->type].atomInfo[atom2->name];
            string contactType2 = info2.inBackbone ? "B" : "S";

            auto dist = (atom1->pos - atom2->pos).norm();
            auto maxOverlapDist = info1.radius + info2.radius;
            if (dist <= maxOverlapDist) {
                auto& contact = contacts[contacts.size()];
                contact.dist0 = dist;
                contact.atom[0] = atom1;
                contact.atom[1] = atom2;
                contact.type = contactType1 + contactType2;
            }
        }
    }
}

Model::Atom& findCA(Model::Residue& res) {
    return find_if(res.atoms.begin(), res.atoms.end(), [](auto const& kv) -> auto {
        return kv.second.name == "CA";
    })->second;
}

mdk::Model Model::reduce() {
    mdk::Model model;

    for (auto& [chainIdx, chain]: chains) {
        auto& mc = model.chains.emplace_back();
        mc.idx = model.chains.size()-1;

        for (auto& [resIdx, res]: chain.residues) {
            auto& atom = mc.atoms.emplace_back();
            atom.chain = &mc;
            atom.idx = mc.atoms.size()-1;
            atom.pos = findCA(res).pos;
            atom.types.push_back(res.type + ".CA");
        }
        auto n = mc.atoms.size();

        mc.tethers = vector<double>(n);
        for (int i = 0; i+1 < n; ++i) {
            auto dist = (mc.atoms[i].pos - mc.atoms[i+1].pos).norm();
            mc.tethers[i] = dist;
        }

        auto& sp = mc.structuredParts.emplace_back();
        sp.offset = 0;
        sp.len = n;

        sp.angle = vector<double>(n);
        sp.dihedral = vector<double>(n);
        for (int i = 1; i+1 < n; ++i) {
            VRef v2 = mc.atoms[i-1].pos, v3 = mc.atoms[i].pos,
                 v4 = mc.atoms[i+1].pos;
            sp.angle[i] = angle(v2, v3, v4);

            if (i > 1) {
                VRef v1 = mc.atoms[i-2].pos;
                sp.dihedral[i] = dihedral(v1, v2, v3, v4);
            }
        }
    }

    return model;
}