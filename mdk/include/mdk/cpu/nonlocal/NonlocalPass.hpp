#pragma once
#include <optional>
#include <mdk/cpu/verlet/VLFactory.hpp>
#include "NativeContacts.hpp"
#include "NonlocalExclusion.hpp"
#include "NonlocalPass.hpp"
#include "PseudoImproperDihedral.hpp"
#include "QuasiAdiabatic.hpp"

namespace mdk {
    class NonlocalPass {
    private:
        VLFactory vlf;

    public:
        std::optional<NativeContacts> natCont;
        std::optional<NonlocalExclusion> nonlocExcl;
        std::optional<PseudoImproperDihedral> pid;
        std::optional<QuasiAdiabatic> qa;

        void init();
        void exec(State const& state, StateDiff& sd) const;
    };
}