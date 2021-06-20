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
        bool cosineVersion = false;

        bool supp(double psi) const;
        void eval(double psi, double& L, double& dL_dpsi) const;
    };

    class PseudoImproperDihedral: public NonlocalForce {
    public:
        LambdaPeak bb_pos, bb_neg, ss;
        LennardJones bb_neg_lj, bb_pos_lj;
        SidechainLJ ss_ljs[AminoAcid::N][AminoAcid::N];
        bool include_i_4 = true;
        bool use_MJ = false;

        void bind(Simulation& simulation) override;
        void asyncPart(Dynamics &) override;
        void vlUpdateHook() override;

    protected:
        vl::Spec spec() const override;

    private:
        Pairs pairs;
        Types const* types = nullptr;
        Chains const* seqs = nullptr;

        void deriveAngles(int i1, int i2,
            double psi[2], Vector dpsi_dr[2][4]) const;
    };
}
