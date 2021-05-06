#pragma once
#include <mdk/tools/model/Model.hpp>
#include <mdk/cpu/generic/LennardJones.hpp>
#include <mdk/cpu/generic/Harmonic.hpp>
#include <vector>

namespace mdk {
    struct NativeContact {
        int i, j;
        double dist0;
        bool isDisulfide;
    };

    class NativeContacts {
    public:
        double effCutoff2;
        LennardJones ljV;
        Harmonic harmV;
        bool useLJForSS;

        std::vector<NativeContact> contacts;

        explicit NativeContacts(Model const& model);

        void perPair(NativeContact& nc, VRef unit, double norm, double& V,
            Vector& dV_dri, Vector& dV_drj);
    };

    inline void NativeContacts::perPair(NativeContact& nc, VRef unit,
            double norm, double &V, Vector &dV_dri, Vector &dV_drj) {

        if (!nc.isDisulfide || useLJForSS) {
            ljV.r_min = nc.dist0;
            ljV.asForce(unit, norm, V, dV_dri, dV_drj);
        }
        else {
            auto diff = norm - nc.dist0;
            harmV.asForce(unit, diff, V, dV_dri, dV_drj);
        }
    }
}