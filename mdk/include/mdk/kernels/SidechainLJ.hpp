#pragma once
#include "../data/Primitives.hpp"
#include "LennardJones.hpp"

namespace mdk {
    class SidechainLJ {
    public:
        double depth = 1.0 * eps;
        double sink_max = 10.0 * angstrom;

        SidechainLJ() = default;
        SidechainLJ(double depth, double sink_max):
            depth(depth), sink_max(sink_max) {};

        inline double cutoff() const {
            return LennardJones(sink_max, depth).cutoff();
        }

        inline void kernel(double norm, double& V, double& dV_dn) const {
            if (norm <= sink_max) V -= depth;
            else LennardJones(sink_max, depth).kernel(norm, V, dV_dn);
        }

        template<typename T1, typename T2>
        inline void asForce(VRef unit, double norm, double& V,
            T1 F1, T2 F2) const {

            double dV_dn = 0.0;
            kernel(norm, V, dV_dn);
            F1 -= dV_dn * unit;
            F2 += dV_dn * unit;
        }
    };
}
