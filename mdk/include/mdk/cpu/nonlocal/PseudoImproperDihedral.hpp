#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/generic/LennardJones.hpp>
#include <mdk/cpu/generic/SinkLJ.hpp>
#include <mdk/cpu/verlet/VL.hpp>
#include <mdk/cpu/data/State.hpp>
#include <array>
#include <mdk/cpu/dynamics/Dynamics.hpp>
#include <mdk/cpu/data/Types.hpp>
#include <mdk/cpu/data/Sequences.hpp>

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
        void deriveAngles(vl::Base const& p, State const& state,
            double psi[2], Vector dpsi_dr[2][6]) const;

        Types types;
        Sequences seqs;

    public:
        Lambda bb_pos, bb_neg, ss;
        LennardJones bb_neg_lj, bb_pos_lj;
        SinkLJ ss_ljs[AminoAcid::N][AminoAcid::N];

        bool include4 = false;

        PseudoImproperDihedral(Model const& model,
            param::Parameters const& params);

        mutable double _cutoff;
        double cutoff() const;

        void perPair(vl::Base const& p,
            State const& state, Dynamics& dyn) const;
    };
}