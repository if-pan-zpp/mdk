#include "cpu/verlet/SimulVL.hpp"
#include "cpu/simul/Simulation.hpp"
using namespace mdk;
using namespace std;

SimulVL::SimulVL(const Simulation& simul) {
    this->simul = &simul;
}

double SimulVL::cutoff() const {
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

void SimulVL::init() {
    pairs = vl::Factory(cutoff(), 5.0 * angstrom);
}

void SimulVL::update() {
    if (pairs.needToReset(simul->state)) {
        pairs.update(simul->state);

        freePairs.clear();
        for (auto const& [i1, i2]: pairs.pairs) {
            auto ch1 = simul->seqs.chainIdx[i1];
            auto ch2 = simul->seqs.chainIdx[i2];

            if (abs(i1 - i2) < 3 && ch1 == ch2 && ch1 >= 0)
                continue;

            vl::Free f;
            f.i1 = i1; f.i2 = i2;
            f.status = vl::Free::Status::FREE;
            freePairs.emplace_back(std::move(f));
        }

        if (simul->natCont) {
            auto natIter = simul->natCont->normals.begin();
            auto natEnd = simul->natCont->normals.end();

            for (auto pIter = freePairs.begin(); pIter != freePairs.end(); ++pIter) {
                auto ix = make_pair(pIter->i1, pIter->i2);

                while (natIter != natEnd && natIter->first < ix)
                    ++natIter;

                if (natIter != natEnd && natIter->first == ix) {
                    pIter->status = vl::Free::Status::TAKEN;
                    nativeContacts.emplace_back(natIter->second);
                }
            }
        }

        std::swap(oldQA, qaContacts);
        qaContacts.clear();
        if (simul->quasiAd) {
            auto qaIter = oldQA.begin();
            auto qaEnd = oldQA.end();

            for (auto pIter = freePairs.begin(); pIter != freePairs.end(); ++pIter) {
                auto ix = make_pair(pIter->i1, pIter->i2);

                while (qaIter != qaEnd && make_pair(qaIter->i1, qaIter->i2) < ix)
                    ++qaIter;

                if (qaIter != qaEnd && make_pair(qaIter->i1, qaIter->i2) == ix) {
                    pIter->status = vl::Free::Status::TAKEN;
                    qaContacts.emplace_back(*qaIter);
                }
            }
        }

        static auto const filter = [](vl::Free const& f) -> auto {
            return f.status == vl::Free::Status::TAKEN;
        };

        freePairs.erase(
            std::remove_if(freePairs.begin(), freePairs.end(), filter),
            freePairs.end());
    }
}
