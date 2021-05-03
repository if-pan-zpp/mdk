#pragma once
#include <mdk/cpu/verlet/VerletList.hpp>
#include <memory>

namespace mdk {
    class VLSpec {
    public:
        double cutoff;
        virtual void filter(VerletList& vl) const = 0;
    };
}
