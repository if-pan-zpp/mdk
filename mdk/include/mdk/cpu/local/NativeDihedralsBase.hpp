#pragma once
#include <mdk/cpu/simul/State.hpp>

namespace mdk {
    class NativeDihedralsBase {
    protected:
        Scalars phi0;

    public:
        explicit NativeDihedralsBase(Model const& model);
    };
}