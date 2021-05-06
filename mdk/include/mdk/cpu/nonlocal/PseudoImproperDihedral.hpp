#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/cpu/data/StateDiff.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <array>

namespace mdk {
    class LambdaPeak {
    public:
        double alpha, psi0;
        bool cosineVersion;

        bool support(double psi) const;
        void compute(double psi, double& lam, double& grad) const;
    };

    class PseudoImproperDihedral {
    private:
        void computeAngles(int i, int j, VRef off, State const& state,
            double psi[2], Vector dpsi_dr[2][4]) const;

        void perLambdaPeak(LambdaPeak const& peak, double depth, double r_min,
            VRef unit, double norm, double psi[2], Vector dpsi_dr[2][4],
            double& V, Vector dV_dr[6]) const;

    public:
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> types;
        LambdaPeak bb_pos, bb_neg, ss;

        double bb_neg_r_min, bb_pos_r_min;

        bool useMJ;
        double ss_depth0, bb_pos_depth, bb_neg_depth;
        double ss_r_min[AminoAcid::numAAs][AminoAcid::numAAs];
        double ss_depth[AminoAcid::numAAs][AminoAcid::numAAs];

        PseudoImproperDihedral(Model const& model,
            param::Parameters const& params);

        void perPair(int i, int j, VRef off, VRef unit, double norm,
            State const& state, StateDiff& sd) const;
    };
}