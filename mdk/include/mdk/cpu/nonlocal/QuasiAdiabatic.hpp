#pragma once
#include <optional>
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/simul/State.hpp>
#include <mdk/cpu/verlet/Base.hpp>
#include <mdk/cpu/data/Stat.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>
#include <mdk/cpu/data/Types.hpp>
#include <mdk/cpu/kernels/SidechainLJ.hpp>
#include <mdk/cpu/data/Sequences.hpp>

namespace mdk {
    struct QAContact: vl::Base {
        enum class Status { FORMING, BREAKING, VACATED, REMOVED } status;
        enum class Type { BB, BS, SB, SS } type;
        double time0;
    };

    class QuasiAdiabatic {
    private:
        Types types;
        param::Parameters::SpecificityParams specs[AminoAcid::N];
        Sequences seqs;

        LennardJones bb_lj, bs_lj;
        SidechainLJ ss_ljs[AminoAcid::N][AminoAcid::N];

        double hr_min, hh_min, nr_max;

        double formationTolerance = 1.0, formationTime = 10.0 * tau;
        double breakingTolerance = 1.0, breakingTime = 10.0 * tau;

    private:
        bool geometryPhase(vl::Free const& p, QAContact& cont) const;

        void forwardStatDiff(vl::Base const& p, QAContact const& cont,
            Stat diffs[2]) const;

        bool coordinationPhase(vl::Base const& p, State const& state,
            QAContact& cont, Stat stats[2]) const;

        void tryBreaking(QAContact& cont, State const& state) const;

        void destroy(QAContact& cont, State& state) const;

    public:
        Vectors n, h;
        bool include4 = false;

        QuasiAdiabatic(Model const& model, param::Parameters const& params);

        double cutoff() const;

        void tryForming(vl::Free& freePair, State& state) const;
        void perPair(QAContact& cont, State& state, Dynamics& dyn) const;
    };
}