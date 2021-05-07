#pragma once
#include <optional>
#include <memory>
#include "Kernels.hpp"
#include <mdk/cpu/integrators/Integrator.hpp>
#include <mdk/cpu/verlet/Factory.hpp>
#include <mdk/cpu/nonlocal/NormalVL.hpp>

namespace mdk {
    class System {
    private:
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> isNative, isConnected;

        State state;
        void localPass(StateDiff& sd) const;
        void verletPass(StateDiff& sd) const;

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

        std::unique_ptr<Integrator> integrator;

    public:
        System() = default;
        explicit System(Model const& model);

        void init();
        void step(double t);
    };
}
