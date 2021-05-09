#pragma once
#include <cstdint>
#include <mdk/cpu/verlet/Factory.hpp>
#include <mdk/cpu/nonlocal/QAContacts.hpp>
#include <mdk/cpu/nonlocal/NativeContacts.hpp>

namespace mdk {
    struct NoContact {};

    class System;

    class SystemVL: public vl::List<NoContact, qa::Contact,
        NativeNormal, NativeDisulfide> {
    private:
        System const* system;

    public:
        explicit SystemVL(System const& system);

        double cutoff() override;
        void refine() override;
    };

    using SystemVLItem = typename SystemVL::Item;

    using SystemVLFactory = vl::Factory<SystemVL,
        NoContact, qa::Contact, NativeNormal, NativeDisulfide>;
}