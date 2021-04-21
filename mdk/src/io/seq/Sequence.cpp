#include "io/seq/Sequence.hpp"
#include "utils/AminoAcid.hpp"
using namespace std;
using namespace mdk;
using namespace mdk::seq;

Model Sequence::asModel(double dist0) const {
    Model model;
    int chainId = 0, atomId = 0;

    for (auto const& chain: chains) {
        auto& model_chain = model.chains.emplace_back();
        model_chain.idx = chainId++;

        for (auto const& code: chain.codes) {
            auto& atom = model.atoms.emplace_back();
            atom.idx = atomId++;
            atom.type = (string)AminoAcid(code) + ".CA";
            atom.pos = { 0.0, 0.0, 0.0 };
            atom.chain = &model_chain;
        }

        for (auto const& spName: chain.structuredParts) {
            auto& sp = structuredParts.at(spName);

            auto& model_sp = model_chain.structuredParts.emplace_back();
            model_sp.offset = sp.offset;
            model_sp.angle = sp.angle;
            model_sp.dihedral = sp.dihedral;

            for (auto const& contact: sp.contacts) {
                auto& model_cont = model.contacts.emplace_back();
                model_cont.type = "BB";
                model_cont.dist = contact.dist;
                for (int i = 0; i < 2; ++i) {
                    int atomIdx = sp.offset + contact.res[i];
                    model_cont.atom[i] = &model.atoms[atomIdx];
                }
            }
        }

        model_chain.tethers = vector<double>(model.atoms.size(), dist0);
    }

    return model;
}
