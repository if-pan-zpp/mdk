#pragma once
#include "DihedralBase.hpp"
#include "../../data/DihedralRanges.hpp"
#include "../../simul/Simulation.hpp"

namespace mdk {
    template<typename Impl>
    class NativeDihedralBase: public DihedralBase<Impl> {
    protected:
        Scalars phi0;

    public:
        void bind(Simulation& simul) {
            Force::bind(simul);

            auto const& dihRanges = simul.data<DihedralRanges>();
            this->ranges = dihRanges.native;
            phi0 = dihRanges.phi_0;
        }
    };
}