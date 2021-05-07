#pragma once
#include <cstdint>
#include <cpu/generic/LennardJones.hpp>
#include <cpu/verlet/List.hpp>
#include <cpu/verlet/Factory.hpp>

namespace mdk {
    struct NoContact {};

    namespace qa {
        enum class Status: int8_t {
            FORMING, BREAKING
        };

        enum class Type: int8_t {
            BB, BS, SB, SS
        };

        struct Contact {
            Status status;
            Type type;
            double time0, r_min;
        };
    }

    struct NormalNC {
        LennardJones ff;
    };

    struct DisulfideNC {};

    namespace sys {
        using VL = vl::List<NoContact, qa::Contact, NormalNC, DisulfideNC>;
        using VLFactory = vl::Factory<NoContact, NoContact, qa::Contact,
            NormalNC, DisulfideNC>;
        using VLItem = typename VL::Item;
    }
}