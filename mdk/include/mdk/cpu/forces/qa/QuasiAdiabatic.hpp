#pragma once
#include <mdk/cpu/forces/NonlocalForce.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/kernels/LennardJones.hpp>
#include <mdk/cpu/kernels/SidechainLJ.hpp>
#include <mdk/cpu/data/Types.hpp>
#include <mdk/cpu/data/Chains.hpp>

namespace mdk {
    class QuasiAdiabatic: public NonlocalForce {
    public:
        QuasiAdiabatic(Model const& model, param::Parameters const& params);

        void onVLUpdate(Pairs& vl, BaseState& state) override;
        void eval(BaseState const& state, BaseDiff& update) const override;
        
        bool checkStat(int i, Stat const& stat) const;

    protected:
        vl::Spec recomputeSpec() const override;

    private:
        Types types;
        param::Parameters::SpecificityParams specs[AminoAcid::N];
        Chains seqs;

        LennardJones bb_lj = LennardJones(5.0 * angstrom, 1.0 * eps);
        LennardJones bs_lj = LennardJones(6.8 * angstrom, 1.0 * eps);
        SidechainLJ ss_ljs[AminoAcid::N][AminoAcid::N];

        Vectors n, h;
        double hr_abs_min = 0.92, hh_abs_min = 0.75, nr_max = 0.5;

        double formationTolerance = 1.0, formationTime = 10.0 * tau;
        double breakingTolerance = 1.0, breakingTime = 10.0 * tau;

        struct PairInfo {
            int i1, i2;
            Vector unit;
            double norm;
        };

        std::vector<QAContact> oldQAContacts;

        bool geometryPhase(PairInfo const& p, QADiff& diff) const;

        void forwardStatDiff(int i1, int i2, QAContact const& diff,
            Stat diffs[2]) const;

        bool coordinationPhase(PairInfo const& p, BaseState const& state,
            QADiff& diff) const;

        void tryForming(PairInfo const& p, BaseState const& state,
            BaseDiff& diff) const;

        void tryBreaking(PairInfo const& p, BaseState const& state,
            QAContact const& cont, BaseDiff& diff) const;

        void destroy(QAContact& cont, BaseDiff& diff) const;
    };
}