#include "tools/model/AtomicModel.hpp"
#include "utils/AminoAcid.hpp"
#include "utils/ResidueTypes.hpp"
#include "utils/Math.hpp"
using namespace std;
using namespace mdk;

AtomicModel::Atom& AtomicModel::addAtom(int serial, Residue *res) {
    auto& atom = atoms[serial];
    atom.serial = serial;
    atom.res = res;
    if (res) {
        atom.idxInRes = res->atoms.size();
        res->atoms.push_back(&atom);
    }
    return atom;
}

AtomicModel::Residue& AtomicModel::addResidue(int serial, Chain *chain) {
    auto& res = residues[serial];
    res.serial = serial;
    res.chain = chain;
    if (chain) {
        res.idxInChain = chain->residues.size();
        chain->residues.push_back(&res);
    }
    return res;
}

AtomicModel::Atom *AtomicModel::Residue::find(std::string const& type) {
    auto iter = find_if(atoms.begin(), atoms.end(),
        [&type](auto const& x) -> auto {
            return x->type == type;
        });

    if (iter != atoms.end()) return *iter;
    else return nullptr;
}

AtomicModel::Chain& AtomicModel::addChain(char serial) {
    auto& chain = chains[serial];
    chain.serial = serial;
    return chain;
}

AtomicModel::Contact& AtomicModel::addContact() {
    auto &cont = contacts.emplace_back();
    cont.idx = contacts.size() - 1;
    return cont;
}

AtomicModel::AtomicModel(Model const& coarse) {
    unordered_map<int, int> resIdxMap;

    for (auto const& chain: coarse.chains) {
        auto& chainHere = addChain('A' + chain.idx);
        for (int resIdx = chain.start; resIdx < chain.end; ++resIdx) {
            auto& res = coarse.residues[resIdx];
            auto& resHere = addResidue(res.idx, &chainHere);
            resHere.type = res.type;

            auto& caAtom = addAtom(atoms.size(), &resHere);
            caAtom.pos = res.pos;
            caAtom.type = "CA";

            resIdxMap[resIdx] = caAtom.idxInRes;
        }
    }

    for (auto const& contact: coarse.contacts) {
        auto& contactHere = addContact();
        contactHere.type = contact.type;
        contactHere.dist0 = contact.dist0;

        for (int i = 0; i < 2; ++i) {
            contactHere.atom[i] = &atoms[resIdxMap[contact.res[i]]];
        }
    }
}

void AtomicModel::addContactsFromAtomOverlap() {
    std::vector<Atom*> contactAtoms;
    for (auto& [idx, atom]: atoms) {
        if (!atom.res) continue;
        if (!AminoAcid::isProper(atom.res->type)) continue;
        contactAtoms.emplace_back(&atom);
    }

    static const auto types = createTypes();
    for (int idx1 = 0; idx1 < contactAtoms.size(); ++idx1) {
        auto *atom1 = contactAtoms[idx1];
        auto const& info1 = types.at((AminoAcid)atom1->res->type)
            .atomInfo.at(atom1->type);
        string type1 = info1.inBackbone ? "B" : "S";

        for (int idx2 = idx1; idx2 < contactAtoms.size(); ++idx2) {
            auto *atom2 = contactAtoms[idx2];
            auto const& info2 = types.at((AminoAcid)atom2->res->type)
                .atomInfo.at(atom2->type);
            string type2 = info2.inBackbone ? "B" : "S";

            if (atom1 >= atom2) continue;
            if (atom1->res == atom2->res) continue;
            if (abs(atom1->res->idxInChain - atom2->res->idxInChain) < 3) continue;

            auto dist = (atom1->pos - atom2->pos).norm();
            auto overlapR = info1.radius + info2.radius;
            if (dist <= overlapR) {
                auto& cont = addContact();
                cont.dist0 = dist;
                cont.type = type1 + type2;
                cont.atom[0] = atom1;
                cont.atom[1] = atom2;
            }
        }
    }
}

void AtomicModel::addContactsFromOnlyCA(double overlap) {
    vector<Atom*> contactAtoms;
    for (auto& [idx, res]: residues) {
        if (!AminoAcid::isProper(res.type)) continue;
        auto *caAtom = res.find("CA");
        if (caAtom) contactAtoms.emplace_back(caAtom);
    }

    for (int idx1 = 0; idx1 < contactAtoms.size(); ++idx1) {
        auto *atom1 = contactAtoms[idx1];

        for (int idx2 = idx1; idx1 < contactAtoms.size(); ++idx2) {
            auto *atom2 = contactAtoms[idx2];

            if (atom1 >= atom2) continue;
            if (atom1->res == atom2->res) continue;
            if (abs(atom1->res->idxInChain - atom2->res->idxInChain) < 3) continue;

            auto dist = (atom1->pos - atom2->pos).norm();
            if (dist <= overlap) {
                auto& cont = addContact();
                cont.dist0 = dist;
                cont.type = "NATIVE";
                cont.atom[0] = atom1;
                cont.atom[1] = atom2;
            }
        }
    }
}

void AtomicModel::addContactsFromResOverlap(const param::Parameters &params) {
    vector<Atom*> contactAtoms;
    for (auto& [idx, res]: residues) {
        if (!AminoAcid::isProper(res.type)) continue;
        auto *caAtom = res.find("CA");
        if (caAtom) contactAtoms.emplace_back(caAtom);
    }

    for (int idx1 = 0; idx1 < contactAtoms.size(); ++idx1) {
        auto *atom1 = contactAtoms[idx1];
        auto rad1 = params.radius.at((AminoAcid)atom1->type);

        for (int idx2 = idx1; idx1 < contactAtoms.size(); ++idx2) {
            auto *atom2 = contactAtoms[idx2];
            auto rad2 = params.radius.at((AminoAcid)atom2->type);

            if (atom1 >= atom2) continue;
            if (atom1->res == atom2->res) continue;
            if (abs(atom1->res->idxInChain - atom2->res->idxInChain) < 3) continue;

            auto dist = (atom1->pos - atom2->pos).norm();
            auto overlap = rad1 + rad2;
            if (dist <= overlap) {
                auto& cont = addContact();
                cont.dist0 = dist;
                cont.type = "NATIVE";
                cont.atom[0] = atom1;
                cont.atom[1] = atom2;
            }
        }
    }
}

Model AtomicModel::coarsen() {
    Model model;
    std::unordered_map<int, int> resIdxMap;
    std::unordered_map<char, int> chainIdxMap;

    for (auto const& [chainIdx, chain]: chains) {
        auto& chainThere = model.addChain();
        chainIdxMap[chain.serial] = chainThere.idx;
        int n = chain.residues.size();

        for (int resIdx = 0; resIdx < n; ++resIdx) {
            auto& res = chain.residues[resIdx];
            auto& resThere = model.addResidue(&chainThere);
            resIdxMap[res->serial] = resThere.idx;
            resThere.type = res->type;
            resThere.pos = res->find("CA")->pos;
        }

        auto& sp = model.addSP();
        chainThere.structuredParts.push_back(sp.idx);
        sp.len = n;
        sp.off = 0;
        sp.angle = vector<double>(n);
        sp.dihedral = vector<double>(n);

        for (int resIdx = 0; resIdx < n; ++resIdx) {
            auto v4 = model.residues.at(resIdx).pos;
            if (resIdx > 0) {
                auto v3 = model.residues.at(resIdx-1).pos;
                if (resIdx > 1) {
                    auto v2 = model.residues.at(resIdx-2).pos;
                    sp.angle[resIdx-1] = angle(v2, v3, v4);
                    if (resIdx > 2) {
                        auto v1 = model.residues.at(resIdx-3).pos;
                        sp.dihedral[resIdx-1] = dihedral(v1, v2, v3, v4);
                    }
                }
            }
        }
    }

    for (auto const& cont: contacts) {
        if (!cont.atom[0]->res || !cont.atom[1]->res) continue;
        auto& contThere = model.addContact();
        contThere.type = cont.type;
        contThere.dist0 = cont.dist0;
        for (int i = 0; i < 2; ++i) {
            contThere.res[i] = resIdxMap.at(cont.atom[i]->res->idxInChain);
        }
    }

    return model;
}
