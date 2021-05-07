#pragma once
#include <mdk/tools/model/Model.hpp>
#include <mdk/cpu/generic/LennardJones.hpp>
#include <mdk/cpu/generic/DisulfideV.hpp>
#include <mdk/cpu/verlet/List.hpp>
#include <vector>
#include <optional>

namespace mdk {
    struct NativeNormal {
        double r_min;
    };

    struct NativeDisulfide {};

    class NativeContacts {
    public:
        double cutoff2 = 0.0 * angstrom;
        DisulfideV disulfideV;

        std::vector<std::tuple<int, int, NativeNormal>> normals;
        std::vector<std::tuple<int, int, NativeDisulfide>> disulfides;

        explicit NativeContacts(Model const& model,
            DisulfideV const& disulfideV);

        void perNormal(vl::Base const& p, NativeNormal& normal,
            double& V, Vector& dV_dr1, Vector& dV_dr2) const;

        void perDisulfide(vl::Base const& p, NativeDisulfide& disulfide,
            double& V, Vector& dV_dr1, Vector& dV_dr2) const;
    };

    inline void NativeContacts::perNormal(vl::Base const& p, NativeNormal& normal,
        double& V, Vector& dV_dr1, Vector& dV_dr2) const {

        thread_local LennardJones lj;
        lj.r_min = normal.r_min;
        lj.asForce(p.unit, p.norm, V, dV_dr1, dV_dr2);
    }

    inline void NativeContacts::perDisulfide(const vl::Base &p,
        NativeDisulfide &disulfide, double &V, Vector &dV_dr1,
        Vector &dV_dr2) const {

        disulfideV.asForce(p.unit, p.norm, V, dV_dr1, dV_dr2);
    }
}