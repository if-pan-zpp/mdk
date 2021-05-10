#include "cpu/simul/SimulVL.hpp"
#include "cpu/simul/Simulation.hpp"
using namespace mdk;
using namespace std;

SimulVL::SimulVL(const Simulation& simul) {
    this->simul = &simul;
}

double SimulVL::cutoff() {
    double maxCutoff = 0;

    if (simul->pauliExcl)
        maxCutoff = max(maxCutoff, simul->pauliExcl->cutoff());

    if (simul->natCont)
        maxCutoff = max(maxCutoff, simul->natCont->cutoff());

    if (simul->pid)
        maxCutoff = max(maxCutoff, simul->pid->cutoff());

    if (simul->quasiAd)
        maxCutoff = max(maxCutoff, simul->quasiAd->cutoff());

    if (simul->constDH)
        maxCutoff = max(maxCutoff, simul->constDH->cutoff());

    if (simul->relativeDH)
        maxCutoff = max(maxCutoff, simul->relativeDH->cutoff());

    return maxCutoff;
}


void SimulVL::refine() {
    basis.erase(std::remove_if(basis.begin(), basis.end(),
        [&](const Boxed<vl::Base>& boxed) -> bool {
            auto& x = boxed.x;
            if (abs(x.i1 - x.i2) >= 3) return false;

            auto chain1 = simul->seqs.chainIdx[x.i1];
            auto chain2 = simul->seqs.chainIdx[x.i2];
            return chain1 == chain2 && chain1 >= 0;
        }), basis.end());

    if (simul->natCont) {
        auto natIter = simul->natCont->normals.begin();
        auto natEnd = simul->natCont->normals.end();

        auto disIter = simul->natCont->disulfides.begin();
        auto disEnd = simul->natCont->disulfides.end();

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