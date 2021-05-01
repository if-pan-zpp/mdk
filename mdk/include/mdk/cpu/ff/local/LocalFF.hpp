#pragma once
#include <optional>
#include <mdk/cpu/ff/ForceField.hpp>
#include "ComplexNativeDihedrals.hpp"
#include "HarmonicTethers.hpp"
#include "HeuresticBondAngles.hpp"
#include "HeuresticDihedrals.hpp"
#include "NativeBondAngles.hpp"
#include "SimpleNativeDihedrals.hpp"
#include "TabularizedBondAngles.hpp"

namespace mdk {
    class LocalFF: public ForceField {
    public:
        std::optional<ComplexNativeDihedrals> cnd;
        std::optional<HarmonicTethers> ht;
        std::optional<HeuresticBondAngles> bha;
        std::optional<HeuresticDihedrals> hd;
        std::optional<NativeBondAngles> nba;
        std::optional<SimpleNativeDihedrals> snd;
        std::optional<TabularizedBondAngles> tba;

        LocalFF();
        void compute(State const& state, double &V, Vectors &dV_dr) override;
    };
}