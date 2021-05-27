#pragma once
#include "NonlocalForce.hpp"
#include "../kernels/LennardJones.hpp"
#include "../kernels/SidechainLJ.hpp"
#include "../data/Types.hpp"
#include "../data/Chains.hpp"
#include "../utils/AminoAcid.hpp"

namespace mdk {
    class LambdaPeak {
    public:
        double alpha, psi0;
        bool cosineVersion;

        bool supp(double psi) const;
        void eval(double psi, double& L, double& dL_dpsi) const;
    };

    class PseudoImproperDihedral: public NonlocalForce {
    private:
        void deriveAngles(int i1, int i2, double norm, VRef unit,
            double psi[2], Vector dpsi_dr[2][6]) const;

        Types const* types = nullptr;
        Chains const* seqs = nullptr;

    public:
        LambdaPeak bb_pos, bb_neg, ss;
        LennardJones bb_neg_lj, bb_pos_lj;
        SidechainLJ ss_ljs[AminoAcid::N][AminoAcid::N];

        void bind(Simulation& simulation) override;
        void asyncPart() override;
        void vlUpdateHook() override;

    protected:
        vl::Spec spec() const override;

    private:
        Pairs pairs;
    };
}