#pragma once
#include <mdk/cpu/data/State.hpp>

namespace mdk {
    class NativeDihedralsBase {
    protected:
        Scalars phi0;

    public:
        NativeDihedralsBase(Model const& model);
    };
}