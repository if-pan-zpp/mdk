#pragma once
#include "../NonlocalForce.hpp"
#include "../../model/Model.hpp"
#include "../../files/param/Parameters.hpp"
#include "../../kernels/LennardJones.hpp"
#include "../../kernels/SidechainLJ.hpp"
#include "../../data/Types.hpp"
#include "../../data/Chains.hpp"
#include "../../stats/Stats.hpp"
#include "../../data/Primitives.hpp"
#include <mutex>

namespace mdk {
    struct QAContact {
        int i1, i2;
        enum class Status: int8_t { FORMING, BREAKING, REMOVED } status;
        Stats::Type type;
        double t0;
    };

    struct QAFreePair {
        int i1, i2;
        enum class Status: int8_t { FREE, TAKEN } status;
    };

    struct QADiff {
        QAContact cont;
        int oldIdx;
        Stat statDiffs[2];

        bool operator<(QADiff const& other) const {
            return oldIdx < other.oldIdx;
        }
    };

    class QuasiAdiabatic: public NonlocalForce {
    public:
        void bind(Simulation &simulation) override;
        void asyncPart(Dynamics &) override;
        void syncPart(Dynamics &) override;
        void vlUpdateHook() override;

    private:
        vl::Spec spec() const override;

        Types const *types = nullptr;
        Chains const *chains = nullptr;

        LennardJones bb_lj = LennardJones(5.0 * angstrom, 1.0 * eps);
        LennardJones bs_lj = LennardJones(6.8 * angstrom, 1.0 * eps);
        SidechainLJ ss_ljs[AminoAcid::N][AminoAcid::N];

        Vectors n, h;
        double formationMaxDistSq;
        double hr_abs_min = 0.92, hh_abs_min = 0.75, nr_max = 0.5;

        double formationTolerance = 1.0, formationTime = 10.0 * tau;
        double breakingTolerance = 1.0, breakingTime = 10.0 * tau;

        std::vector<QAContact> oldPairs, pairs;
        std::vector<QAFreePair> freePairs;

        Stats *stats;
        std::mutex qaDiffsMutex;
        std::vector<QADiff> qaDiffs;

        struct PairInfo {
            int i1, i2;
            Vector unit;
            double norm;
        };

        void computeNH();
        bool geometryPhase(PairInfo const& p, QADiff& diff) const;
    };
}
