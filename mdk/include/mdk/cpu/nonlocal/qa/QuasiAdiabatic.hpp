#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/data/State.hpp>
#include <mdk/cpu/data/StateDiff.hpp>
#include <mdk/cpu/verlet/List.hpp>
#include <cpu/nonlocal/NormalVL.hpp>
#include "Stats.hpp"

namespace mdk {
    using namespace param;

    class QuasiAdiabatic {
    private:
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> types;
        Parameters::SpecificityParams specs[AminoAcid::N];

        double bb_cutoff, bs_cutoff;
        double ss_cutoff[AminoAcid::N][AminoAcid::N];
        double hr_min, hh_min, nr_max;

        double formationTolerance, breakingTolerance;
        double maxDepth, formationTime, breakingTime;

    private:
        bool dirPhase(vl::Pair<NormalData>& p) const;
        bool distPhase(vl::Pair<NormalData>& p) const;
        void computeDiff(vl::Pair<NormalData> const& p, qa::Stat stat[2]) const;
        bool coordPhase(vl::Pair<NormalData>& p, qa::Stats const& stats,
            qa::Diff& diff) const;

        void tryCreate(vl::Pair<NormalData> const& p,
            State const& state, StateDiff& sd) const;

        void tryDestroy(vl::Pair<NormalData> const& p,
            State const& state, StateDiff& sd) const;

        void removePair(vl::Pair<NormalData> const& p,
            StateDiff& sd) const;

    public:
        Vectors n, h;

        QuasiAdiabatic(Model const& model, Parameters const& params);

        void perPair(vl::Pair<NormalData> const& p,
            State const& state, StateDiff& sd) const;
    };
}