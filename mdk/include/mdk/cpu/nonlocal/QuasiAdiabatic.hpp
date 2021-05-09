#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/data/State.hpp>
#include <mdk/cpu/verlet/VL.hpp>
#include <optional>
#include <mdk/cpu/data/Stat.hpp>
#include <mdk/cpu/generic/DisulfideV.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>
#include <mdk/cpu/sys/SystemVL.hpp>
#include <mdk/cpu/data/Types.hpp>
#include <mdk/cpu/generic/SinkLJ.hpp>
#include <mdk/cpu/data/Sequences.hpp>

namespace mdk {
    class QuasiAdiabatic {
    private:
        Types types;
        param::Parameters::SpecificityParams specs[AminoAcid::N];
        Sequences seqs;

        LennardJones bb_lj, bs_lj;
        SinkLJ ss_ljs[AminoAcid::N][AminoAcid::N];

        double hr_min, hh_min, nr_max;

        double formationTolerance, formationTime;
        double breakingTolerance, breakingTime;

    private:
        bool directionPhase(vl::Base const& p, qa::Contact& cont) const;

        bool distancePhase(vl::Base const& p, qa::Contact& cont) const;

        void forwardStatDiff(vl::Base const& p, qa::Contact const& cont,
            Stat diffs[2]) const;

        bool coordinationPhase(vl::Base const& p, State const& state,
            qa::Contact& cont, Stat stats[2]) const;

        void tryBreaking(vl::Base const& p, State const& state,
            qa::Contact& cont) const;

        void destroy(SystemVLItem& item, State& state) const;

    public:
        Vectors n, h;
        bool include4 = false;

        QuasiAdiabatic(Model const& model, param::Parameters const& params);

        mutable double _cutoff;
        double cutoff() const;

        void tryForming(SystemVLItem& item, State& state) const;
        void perPair(SystemVLItem& item, State& state, Dynamics& dyn) const;
    };
}