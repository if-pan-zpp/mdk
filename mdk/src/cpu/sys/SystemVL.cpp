#include "cpu/sys/SystemVL.hpp"
#include "cpu/sys/System.hpp"
using namespace mdk;
using namespace std;

SystemVL::SystemVL(const System &system) {
    this->system = &system;
}

double SystemVL::cutoff() {
    double maxCutoff = 0;

    if (system->pauliExcl)
        maxCutoff = max(maxCutoff, system->pauliExcl->cutoff());

    if (system->natCont)
        maxCutoff = max(maxCutoff, system->natCont->cutoff());

    if (system->pid)
        maxCutoff = max(maxCutoff, system->pid->cutoff());

    if (system->quasiAd)
        maxCutoff = max(maxCutoff, system->quasiAd->cutoff());

    if (system->constDH)
        maxCutoff = max(maxCutoff, system->constDH->cutoff());

    if (system->relativeDH)
        maxCutoff = max(maxCutoff, system->relativeDH->cutoff());

    return maxCutoff;
}


void SystemVL::refine() {
    basis.erase(std::remove_if(basis.begin(), basis.end(),
        [&](const Boxed<vl::Base>& boxed) -> bool {
            auto& x = boxed.x;
            if (abs(x.i1 - x.i2) >= 3) return false;

            auto chain1 = system->seqs.chainIdx[x.i1];
            auto chain2 = system->seqs.chainIdx[x.i2];
            return chain1 == chain2 && chain1 >= 0;
        }), basis.end());

    if (system->natCont) {
        auto natIter = system->natCont->normals.begin();
        auto natEnd = system->natCont->normals.end();

        auto disIter = system->natCont->disulfides.begin();
        auto disEnd = system->natCont->disulfides.end();

        for (auto& item: *this) {
            auto ix = make_pair(item.base().i1, item.base().i2);

            while (natIter != natEnd && natIter->first < ix) ++natIter;
            if (natIter != natEnd && natIter->first == ix)
                item.morph<NativeNormal>(natIter->second);

            while (disIter != disEnd && disIter->first < ix) ++disIter;
            if (disIter != disEnd && disIter->first == ix)
                item.morph<NativeDisulfide>(disIter->second);
        }
    }
}