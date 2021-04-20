#include "mdk/io/pdb/Data.hpp"
#include <mdk/utils/AminoAcid.hpp>
using namespace std;
using namespace mdk;
using namespace mdk::pdb;
using namespace mdk::pdb::records;

Data::Data(const Model &model) {
    for (auto const& [chainID, chain]: model.chains) {
        for (auto const& [resID, res]: chain.residues) {
            Record record = Atom();
            auto& data = get<Atom>(record);

            for (auto const& [atomID, atom]: res.atoms) {
                data.serialNum = atomID;
                data.atomName = atom.name;
                data.residueName = atom.res->type;
                data.chainID = chainID;
                data.residueSeqNum = resID;
                data.pos = atom.pos;
                data.element = atom.element;

                records.push_back(record);
            }
        }
    }

    for (auto const& [bondID, bond]: model.ssbonds) {
        if (bond.res[0]->type == "CYS" && bond.res[1]->type == "CYS") {
            Record record = SSBond();
            auto& data = get<SSBond>(record);

            data.eqDist = bond.dist0;
            data.serialNum = bondID;
            for (int i = 0; i < 2; ++i) {
                data.res[i].residueSeqNum = bond.res[i]->id;
                data.res[i].chainId = bond.res[i]->chain->id;
            }

            records.push_back(record);
        }
    }

    records.emplace_back(End());
}

pdb::Model Data::asModel() const {
    pdb::Model model;

    for (auto const& record: records) {
        if (holds_alternative<Atom>(record)) {
            auto& data = get<Atom>(record);

            if (model.chains.find(data.chainID) == model.chains.end()) {
                auto& chain = model.chains[data.chainID];
                chain.id = data.chainID;
            }
            auto& chain = model.chains[data.chainID];

            if (chain.residues.find(data.residueSeqNum) == chain.residues.end()) {
                auto& res = chain.residues[data.residueSeqNum];
                res.id = data.residueSeqNum;
                res.type = data.residueName;
                res.chain = &chain;
            }
            auto& res = chain.residues[data.residueSeqNum];

            if (res.atoms.find(data.serialNum) == res.atoms.end()) {
                auto& atom = res.atoms[data.serialNum];
                atom.id = data.serialNum;
                atom.element = data.element;
                atom.pos = data.pos;
                atom.name = data.atomName;
            }
        }
        else if (holds_alternative<SSBond>(record)) {
            auto const& data = get<SSBond>(record);

            if (model.ssbonds.find(data.serialNum) == model.ssbonds.end()) {
                auto& bond = model.ssbonds[data.serialNum];
                for (int i = 0; i < 2; ++i) {
                    auto& resData = data.res[i];
                    auto& chain = model.chains[resData.chainId];
                    auto& res = chain.residues[resData.residueSeqNum];
                    bond.res[i] = &res;
                }
                bond.dist0 = data.eqDist;
            }
        }
        else if (holds_alternative<Cryst1>(record)) {
            auto const& data = get<Cryst1>(record);
            model.cellDims = data.size;
        }
        else if (holds_alternative<End>(record)) {
            break;
        }
    }

    return model;
}
