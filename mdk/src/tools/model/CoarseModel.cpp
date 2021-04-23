#include "tools/model/CoarseModel.hpp"
using namespace std;
using namespace mdk;

CoarseModel::Residue& CoarseModel::addResidue(Chain *chain) {
    auto& res = residues.emplace_back();
    res.idx = residues.size()-1;
    if (chain) {
        res.chainIdx = chain->idx;
        if (chain->first < 0) chain->first = 0;
        chain->last = res.idx+1;
    }
    return res;
}

CoarseModel::Chain& CoarseModel::addChain() {
    auto& chain = chains.emplace_back();
    chain.idx = chains.size()-1;
    chain.first = chain.last = -1;
    return chain;
}

CoarseModel::Contact &CoarseModel::addContact() {
    auto& cont = contacts.emplace_back();
    cont.idx = contacts.size()-1;
    return cont;
}

CoarseModel::StructuredPart& CoarseModel::addSP() {
    auto& sp = structuredParts.emplace_back();
    sp.idx = structuredParts.size()-1;
    return sp;
}

void CoarseModel::morphIntoLine(double tether0) {
    Eigen::Vector3d cur = { 0.0, 0.0, 0.0 };

    Eigen::Vector3d dir = { 0.0, 0.0, tether0 };
    for (auto& res: residues) {
        res.pos = res.idx * dir;
    }
}

void CoarseModel::morphIntoSAW(Random *rand, double density, bool withPBC, double intersectionDist) {

}
