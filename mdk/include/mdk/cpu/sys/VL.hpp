#pragma once
#include <cstdint>
#include <cpu/verlet/Factory.hpp>
#include <cpu/nonlocal/QAContacts.hpp>
#include <cpu/nonlocal/NativeContacts.hpp>

namespace mdk {
    struct NoContact {};

    class System;

    class SystemVL: public vl::List<NoContact, qa::Contact,
        NativeNormal, NativeDisulfide> {
    private:
        System const* owner = nullptr;

    public:
        SystemVL() = default;
        SystemVL(System const& sys);

        double cutoff2() override;
        void refine() override;
    };

    using SystemVLItem = typename SystemVL::Item;

    using SystemVLFactory = vl::List<NoContact, qa::Contact, NativeNormal,
        NativeDisulfide>;
}