#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/cpu/kernels/LennardJones.hpp>

namespace mdk {
    class SidechainLJ {
    public:
        double depth = 1.0 * eps;
        double sink_min = 4.0 * angstrom;
        double sink_max = 10.0 * angstrom;

        SidechainLJ() = default;
        SidechainLJ(double depth, double sink_min, double sink_max):
            depth(depth), sink_min(sink_min), sink_max(sink_max) {};

        inline double cutoff() const {
            return LennardJones(sink_max, depth).cutoff();
        }

        inline void kernel(double norm, double& V, double& dV_dn) const {
            if (sink_min <= norm && norm <= sink_max) {
                V -= depth;
            }
            else {
                auto norm_inv = 1.0 / norm, s = norm_inv;
                if (norm < sink_min) s *= sink_min;
                else s *= sink_max;

                auto s6 = s*s*s*s*s*s, s12 = s6*s6;
                V += depth * (s12 - 2.0 * s6);
                dV_dn += depth * (s6 - s12) * norm_inv;
            }
        }

        template<typename T1, typename T2>
        inline void asForce(VRef unit, double norm, double& V,
            T1 F1, T2 F2) const {

            double dV_dn = 0.0;
            kernel(norm, V, dV_dn);
            F1 += dV_dn * unit;
            F2 -= dV_dn * unit;
        }
    };
}
