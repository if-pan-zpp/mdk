#pragma once
#include <optional>
#include <mdk/cpu/data/State.hpp>
#include <mdk/cpu/data/StateDiff.hpp>
#include "ComplexNativeDihedrals.hpp"
#include "HarmonicTethers.hpp"
#include "HeuresticBondAngles.hpp"
#include "HeuresticDihedrals.hpp"
#include "NativeBondAngles.hpp"
#include "SimpleNativeDihedrals.hpp"
#include "TabularizedBondAngles.hpp"
#include "Chirality.hpp"
#include <cpu/local/LocalExclusion.hpp>

namespace mdk {
    class LocalPass {
    private:
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> spIdx;
        Scalars theta0, phi0;
        Regions ranges;

    public:
        std::optional<ComplexNativeDihedrals> compNativeDih;
        std::optional<HarmonicTethers> harmonic;
        std::optional<HeuresticBondAngles> heurBA;
        std::optional<HeuresticDihedrals> heurDih;
        std::optional<NativeBondAngles> nativeBA;
        std::optional<SimpleNativeDihedrals> simpNativeDih;
        std::optional<TabularizedBondAngles> tabBA;
        std::optional<Chirality> chir;
        std::optional<LocalExclusion> locExcl;
        Vectors* hat_rij_x_rjk;

        explicit LocalPass(Model const& model);

        void init();
        void exec(State const& state, StateDiff& sd) const;
    };
}
