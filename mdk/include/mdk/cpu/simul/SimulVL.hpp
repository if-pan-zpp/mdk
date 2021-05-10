#pragma once
#include <cstdint>
#include <mdk/cpu/verlet/Factory.hpp>
#include <mdk/cpu/nonlocal/QAContacts.hpp>
#include <mdk/cpu/nonlocal/NativeContacts.hpp>

namespace mdk {
    struct NoContact {};

    class Simulation;

    class SimulVL: public vl::List<NoContact, qa::Contact,
        NativeNormal, NativeDisulfide> {
    private:
        Simulation const* simul;

    public:
        explicit SimulVL(Simulation const& simul);

        double cutoff() override;
        void refine() override;
    };

    using SystemVLItem = typename SimulVL::Item;

    using SystemVLFactory = vl::Factory<SimulVL,
        NoContact, qa::Contact, NativeNormal, NativeDisulfide>;
}