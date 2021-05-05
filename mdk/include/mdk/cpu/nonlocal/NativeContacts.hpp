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

        NativeContacts(Model const& model);

        void eval(NativeContact& nc, VRef unit, double norm, double& V,
            Vector& dV_dri, Vector& dV_drj);
    };

    inline void NativeContacts::eval(NativeContact& nc, VRef unit,
            double norm, double &V, Vector &dV_dri, Vector &dV_drj) {
        auto diff = norm - nc.dist0;

        if (!nc.isDisulfide || useLJForSS) {
            ljV.eval(unit, diff, V, dV_dri, dV_drj);
        }
        else {
            harmV.eval(unit, diff, V, dV_dri, dV_drj);
        }
    }
}