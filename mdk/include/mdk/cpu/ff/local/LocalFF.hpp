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
#include "Chirality.hpp"
#include "LocalExclusion.hpp"

namespace mdk {
    class LocalFF: public ForceField {
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

        explicit LocalFF(Model const& model);
        void compute(State const& state, double &V, Vectors &dV_dr) override;
    };
}
