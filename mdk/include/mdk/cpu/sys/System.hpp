#pragma once
#include <optional>
#include <memory>
#include "Kernels.hpp"
#include <mdk/cpu/sys/VL.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>
#include <mdk/cpu/dynamics/LangevinDynamics.hpp>
#include <mdk/cpu/dynamics/PredictorCorrector.hpp>
#include <mdk/cpu/dynamics/Leapfrog.hpp>

namespace mdk {
    class System {
    private:
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> isNative, isConnected;

        State state;

        void localPass();
        void verletPass();

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
        System() = default;
        explicit System(Model const& model);

        void init();
        void step(double t);
    };
}
