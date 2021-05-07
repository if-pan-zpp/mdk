#pragma once
#include <cstdint>
#include <cpu/verlet/Factory.hpp>
#include <cpu/nonlocal/QAContacts.hpp>
#include <cpu/nonlocal/NativeContacts.hpp>

namespace mdk {
    struct NoContact {};

    class System;

    class SysVL: public vl::List<NoContact, qa::Contact,
        NativeNormal, NativeDisulfide> {
    private:
        System const* owner;

    public:
        SysVL() = default;
        SysVL(System const& sys);

        double cutoff2() override;
        void refine() override;
    };

    using SysVLItem = typename SysVL::Item;

    using SysVLFactory = vl::Factory<NoContact, qa::Contact, NativeNormal,
    NativeDisulfide>;
}