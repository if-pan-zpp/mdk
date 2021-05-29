#pragma once
#include "../../simul/SimulVar.hpp"
#include "../../simul/Simulation.hpp"

namespace mdk {
    class NativeDihedralBase: public SimulVar {
    protected:
        Bytes isNative;
        Scalars phi0;

    public:
        void bind(Simulation& simulation) override;
    };
}