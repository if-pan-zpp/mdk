#pragma once
#include <cpu/data/Primitives.hpp>
#include <tools/model/Model.hpp>
#include <tools/param/Parameters.hpp>
#include <cpu/nonlocal/NormalVL.hpp>

namespace mdk {
    class DebyeHueckel {
    private:
        Eigen::Matrix<int8_t, Eigen::Dynamic, 1> charge;

    public:
        double screeningDist;
        bool useConstantPerm;
        double perm;

        DebyeHueckel(Model const& model, param::Parameters const& params);

        void asForce(vl::Pair<NormalData> const& p, double& V,
            Vector& dV_dr1, Vector& dV_dr2) const;
    };

    inline void DebyeHueckel::asForce(vl::Pair<NormalData> const& p, double& V,
            Vector& dV_dr1, Vector& dV_dr2) const {
        double C = charge[p.i1] * charge[p.i2];
        if (C != 0.0) {
            C /= 4 * M_PI * perm;
            double v = C * exp(-p.norm / screeningDist) / p.norm;
            V += v;
            double dV_dn = v * (1.0 + p.norm / screeningDist) / p.norm;
            dV_dr1 -= dV_dn * p.unit;
            dV_dr2 += dV_dn * p.unit;
        }
    }
}