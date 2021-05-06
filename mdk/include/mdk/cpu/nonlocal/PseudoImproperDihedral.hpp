#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/cpu/data/StateDiff.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/generic/LennardJones.hpp>
#include <cpu/generic/SinkLJ.hpp>
#include <array>

namespace mdk {
    class Lambda {
    public:
        double alpha, psi0;
        bool cosineVersion;

        bool supp(double psi) const;
        void eval(double psi, double& L, double& dL_dpsi) const;
    };

    class PseudoImproperDihedral {
    private:
        void deriveAngles(vl::Pair<NormalData> const& p, State const& state,
            double psi[2], Vector dpsi_dr[2][6]) const;

        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> types;

    public:
        Lambda bb_pos, bb_neg, ss;
        LennardJones bb_neg_lj, bb_pos_lj;
        SinkLJ ss_ljs[AminoAcid::N][AminoAcid::N];

        PseudoImproperDihedral(Model const& model,
            param::Parameters const& params);

        void perPair(vl::Pair<NormalData> const& p,
            State const& state, StateDiff& sd) const;
    };
}