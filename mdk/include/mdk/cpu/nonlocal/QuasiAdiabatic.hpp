#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/data/State.hpp>
#include <mdk/cpu/verlet/List.hpp>
#include <optional>
#include "cpu/data/Stat.hpp"
#include <mdk/cpu/generic/DisulfideV.hpp>
#include <cpu/dynamics/Dynamics.hpp>
#include "cpu/sys/VL.hpp"
#include <cpu/data/Types.hpp>

namespace mdk {
    class QuasiAdiabatic {
    private:
        Types types;
        param::Parameters::SpecificityParams specs[AminoAcid::N];

        double bb_cutoff, bs_cutoff;
        double ss_cutoff[AminoAcid::N][AminoAcid::N];
        double max_cutoff;

        double hr_min, hh_min, nr_max;

        double formationTolerance, breakingTolerance;
        double maxDepth, formationTime, breakingTime;

    private:
        bool directionPhase(vl::Base const& p, State const& state,
            qa::Contact& cont) const;

        bool distancePhase(vl::Base const& p, State const& state,
            qa::Contact& cont) const;

        void forwardStatDiff(vl::Base const& p, qa::Contact const& cont,
            Stat diffs[2]) const;

        bool coordinationPhase(vl::Base const& p, State const& state,
            qa::Contact& cont, Stat stats[2]) const;

        void tryBreaking(vl::Base const& p, State const& state,
            qa::Contact& cont) const;

        void destroy(SystemVLItem& item, State& state) const;

    public:
        Vectors n, h;

        QuasiAdiabatic(Model const& model, param::Parameters const& params);

        void tryForming(SystemVLItem& item, State& state) const;
        void perPair(SystemVLItem& item, State& state, Dynamics& dyn) const;
    };
}