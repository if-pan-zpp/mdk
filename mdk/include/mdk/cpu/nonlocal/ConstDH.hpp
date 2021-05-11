#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/data/Charges.hpp>
#include <mdk/cpu/verlet/Base.hpp>

namespace mdk {
    class ConstDH {
    private:
        Charges charge;

    public:
        double screeningDist = 10.0 * angstrom;
        double perm = 80.0 * epsilon_0;

        inline double cutoff() const {
            return screeningDist;
        }

        explicit ConstDH(Charges charge):
            charge(std::move(charge)) {};

        inline void perPair(vl::Base const& p, Dynamics& dyn) const {
            auto q1 = charge[p.i1], q2 = charge[p.i2];
            if (q1 != 0.0 && q2 != 0.0) {
                auto coeff = (q1 * q2) / (4.0 * M_PI * perm);
                double V_DH = coeff * exp(-p.norm/screeningDist) / p.norm;
                dyn.V += V_DH;

                double dV_dn = -V_DH * (1.0 + p.norm/screeningDist)/p.norm;
                dyn.F[p.i1] += dV_dn * p.unit;
                dyn.F[p.i2] -= dV_dn * p.unit;
            }
        }
    };
}