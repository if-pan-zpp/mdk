#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/data/State.hpp>
#include <mdk/cpu/data/StateDiff.hpp>
#include <cpu/qa/ContactType.hpp>

namespace mdk {
    using namespace param;

    class QuasiAdiabatic {
    private:
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> types;
        Parameters::SpecificityParams specs[AminoAcid::numAAs];

        double bb_cutoff, bs_cutoff;
        double ss_cutoff[AminoAcid::numAAs][AminoAcid::numAAs];
        double hr_min, hh_min, nr_max;

        double formationTolerance, breakingTolerance;
        double maxDepth, formationTime, breakingTime;

    private:
        bool dirPhase(vl::Pair<NormalData>& p, VRef unit12) const;
        bool distPhase(vl::Pair<NormalData>& p, double norm) const;
        void adjustStats(vl::Pair<NormalData> const& p, qa::Stat* stat,
            int dir) const;
        bool coordPhase(vl::Pair<NormalData>& p, qa::Stats const& stats,
            qa::Diff& diff) const;

        void tryCreate(vl::Pair<NormalData> const& p, VRef unit12, double norm,
            State const& state, StateDiff& sd) const;

        void tryDestroy(vl::Pair<NormalData> const& p, double norm,
            State const& state, StateDiff& sd) const;

        void removePair(vl::Pair<NormalData> const& p,
            State const& state, StateDiff& sd) const;

    public:
        mutable Vectors n, h;

        QuasiAdiabatic(Model const& model, Parameters const& params);

        void perPair(vl::Pair<NormalData> const& p, VRef unit12, double norm,
            State const& state, StateDiff& sd) const;
    };
}