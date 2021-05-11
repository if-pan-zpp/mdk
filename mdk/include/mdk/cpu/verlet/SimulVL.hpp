#pragma once
#include <cstdint>
#include <vector>
#include <mdk/cpu/verlet/Pairs.hpp>
#include <mdk/cpu/nonlocal/NativeContacts.hpp>
#include <mdk/cpu/nonlocal/QuasiAdiabatic.hpp>

namespace mdk {
    class Simulation;

    class SimulVL {
    private:
        friend class Simulation;
        Simulation const* simul;
        vl::Pairs pairs;

        std::vector<vl::Free> freePairs;
        std::vector<NativeContact> nativeContacts;
        std::vector<QAContact> oldQA, qaContacts;

        [[nodiscard]] double cutoff() const;

    public:
        explicit SimulVL(Simulation const& simul);

        void init();
        void update();
    };
}