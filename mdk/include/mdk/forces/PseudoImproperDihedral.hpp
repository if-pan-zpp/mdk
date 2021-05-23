#pragma once
#include <forces/NonlocalForce.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <kernels/LennardJones.hpp>
#include <kernels/SidechainLJ.hpp>
#include <data/Types.hpp>
#include <data/Chains.hpp>

namespace mdk {
    class Lambda {
    public:
        double alpha, psi0;
        bool cosineVersion;

        bool supp(double psi) const;
        void eval(double psi, double& L, double& dL_dpsi) const;
    };

    class PseudoImproperDihedral: public NonlocalForce {
    private:
        void deriveAngles(int i1, int i2, double norm, VRef unit,
            BaseState const& state, double psi[2], Vector dpsi_dr[2][6]) const;

        Types types;
        Chains seqs;

    public:
        Lambda bb_pos, bb_neg, ss;
        LennardJones bb_neg_lj, bb_pos_lj;
        SidechainLJ ss_ljs[AminoAcid::N][AminoAcid::N];

        PseudoImproperDihedral(Model const& model,
            param::Parameters const& params);

        void reconstructFrom(Pairs& vl) override;
        void eval(BaseState const& state, BaseDiff& update) const override;

    protected:
        vl::Spec recomputeSpec() const override;

    private:
        Pairs pairs;
    };
}