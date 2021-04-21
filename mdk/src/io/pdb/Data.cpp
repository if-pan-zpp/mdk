#include "mdk/io/pdb/Data.hpp"
#include <mdk/utils/AminoAcid.hpp>
using namespace std;
using namespace mdk;
using namespace mdk::pdb;
using namespace mdk::pdb::records;

static bool isDisulfideBondEnd(pdb::Model::Atom const& atom) {
    return atom.name == "SG" && atom.res->type == "CYS";
}

static bool isDisulfideBond(pdb::Model::Contact const& contact) {
    return isDisulfideBondEnd(*contact.atom[0]) &&
           isDisulfideBondEnd(*contact.atom[1]);
}

Data::Data(const Model &model) {
    for (auto const& [chainID, chain]: model.chains) {
        for (auto const& [resIdx, res]: chain.residues) {
            Record record = Atom();
            auto& data = get<Atom>(record);

            for (auto const& [atomIdx, atom]: res.atoms) {
                data.serialNum = atom.idx;
                data.atomName = atom.name;
                data.residueName = atom.res->type;
                data.chainID = chainID;
                data.residueSeqNum = res.idx;
                data.pos = atom.pos;
                data.element = atom.element;

                records.push_back(record);
            }
        }
    }

    for (auto const& [id, contact]: model.contacts) {
        if (isDisulfideBond(contact)) {
            Record record = SSBond();
            auto& data = get<SSBond>(record);

            data.eqDist = contact.dist0;
            data.serialNum = contact.idx;
            for (int i = 0; i < 2; ++i) {
                data.res[i].residueSeqNum = contact.atom[i]->idx;
                data.res[i].chainId = contact.atom[i]->res->chain->idx;
            }

            records.push_back(record);
        }
    }

    records.emplace_back(End());
}

static pdb::Model::Atom& findSG(pdb::Model::Residue& res) {
    return find_if(res.atoms.begin(), res.atoms.end(), [](auto const& kv) -> auto {
        return kv.second.name == "SG";
    })->second;
}

pdb::Model Data::asModel() const {
    pdb::Model model;

    for (auto const& record: records) {
        if (holds_alternative<Atom>(record)) {
            auto& data = get<Atom>(record);

            if (model.chains.find(data.chainID) == model.chains.end()) {
                auto& chain = model.chains[data.chainID];
                chain.idx = data.chainID;
            }
            auto& chain = model.chains[data.chainID];

            if (chain.residues.find(data.residueSeqNum) == chain.residues.end()) {
                auto& res = chain.residues[data.residueSeqNum];
                res.idx = data.residueSeqNum;
                res.type = data.residueName;
                res.chain = &chain;
            }
            auto& res = chain.residues[data.residueSeqNum];

            if (res.atoms.find(data.serialNum) == res.atoms.end()) {
                auto& atom = res.atoms[data.serialNum];
                atom.idx = data.serialNum;
                atom.element = data.element;
                atom.pos = data.pos;
                atom.name = data.atomName;
                atom.res = &res;
            }
        }
        else if (holds_alternative<SSBond>(record)) {
            auto const& data = get<SSBond>(record);

            if (model.contacts.find(data.serialNum) == model.contacts.end()) {
                auto& bond = model.contacts[data.serialNum];
                for (int i = 0; i < 2; ++i) {
                    auto& resData = data.res[i];
                    auto& chain = model.chains[resData.chainId];
                    auto& res = chain.residues[resData.residueSeqNum];
                    auto& atom = findSG(res);
                    bond.atom[i] = &atom;
                }
                bond.dist0 = data.eqDist;
                bond.type = "SSBOND";
            }
        }
        else if (holds_alternative<Cryst1>(record)) {
            auto const& data = get<Cryst1>(record);
            model.cell = data.size;
        }
        else if (holds_alternative<End>(record)) {
            break;
        }
    }

    return model;
}
