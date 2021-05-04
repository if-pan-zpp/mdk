#pragma once
#include <optional>
#include "Kernels.hpp"

namespace mdk {
    class ForceField {
    private:
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> spIdx;
        Ranges chains;
        void localPass(State const& state, StateDiff& sd) const;

    public:
        template<typename T>
        using opt = std::optional<T>;

        opt<HarmonicTethers> harm;
        opt<Chirality> chir;
        opt<NativeBondAngles> nativeBA;
        opt<HeuresticBondAngles> heurBA;
        opt<SimpleNativeDihedrals> simpNativeDih;
        opt<ComplexNativeDihedrals> compNativeDih;
        opt<HeuresticDihedrals> heurDih;

        opt<ShiftedTruncatedLJ> pauliExcl;
        opt<NativeContacts> nativeCont;
        opt<PseudoImproperDihedral> pid;
        opt<QuasiAdiabatic> quasiAd;

    public:
        explicit ForceField(Model const& model);

        void eval(State const& state, StateDiff& sd) const;
    };
}
