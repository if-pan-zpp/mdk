#pragma once
#include <optional>
#include <memory>
#include <mdk/cpu/verlet/SimulVL.hpp>
#include <mdk/cpu/simul/Imports.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>
#include <mdk/cpu/dynamics/LangevinDynamics.hpp>
#include <mdk/cpu/dynamics/PredictorCorrector.hpp>
#include <mdk/cpu/dynamics/Leapfrog.hpp>
#include <mdk/cpu/data/Sequences.hpp>

namespace mdk {
    class Simulation {
    public:
        friend class SimulVL;

        Sequences seqs;
        SimulVL vl;

        bool interrupt = false;
        double cutoff_sq;
        State state;
        Dynamics dyn;

        bool initialized = false;
        bool equilibrationPhase = false;
        void localPass();
        void verletPass();

        bool coherencyCheck() const;

    public:
        std::optional<HarmonicTethers> harm;
        std::optional<Chirality> chir;
        std::optional<NativeBondAngles> nativeBA;
        std::optional<HeuresticBondAngles> heurBA;
        std::optional<SimpleNativeDihedrals> simpNativeDih;
        std::optional<ComplexNativeDihedrals> compNativeDih;
        std::optional<HeuresticDihedrals> heurDih;
        std::vector<SolidWall> walls;

        std::optional<PauliExclusion> pauliExcl;
        std::optional<NativeContacts> natCont;
        std::optional<PseudoImproperDihedral> pid;
        std::optional<QuasiAdiabatic> quasiAd;
        std::optional<ConstDH> constDH;
        std::optional<RelativeDH> relativeDH;

        std::optional<LangevinDynamics> langDyn;
        std::optional<PredictorCorrector> pc;
        std::optional<Leapfrog> leapfrog;

        std::vector<std::shared_ptr<Hook>> hooks;

    public:
        explicit Simulation(Model const& model);

        void init();
        void step(int n = 1);
        void step(double t);
    };
}
