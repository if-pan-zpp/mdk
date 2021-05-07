#pragma once
#include <mdk/tools/model/Model.hpp>
#include <mdk/cpu/generic/LennardJones.hpp>
#include <mdk/cpu/generic/DisulfideV.hpp>
#include <mdk/cpu/verlet/List.hpp>
#include <vector>
#include <optional>

namespace mdk {

    class NativeContacts {
    public:
        double cutoff2 = 0.0 * angstrom;
        DisulfideV disulfideV;

        std::vector<std::tuple<int, int, NormalNC>> normals;
        std::vector<std::tuple<int, int, DisulfideNC>> disulfides;

        explicit NativeContacts(Model const& model,
            DisulfideV const& disulfideV);

        void perNormal(vl::Base const& p, NormalNC& normal,
            double& V, Vector& dV_dr1, Vector& dV_dr2) const;

        void perDisulfide(vl::Base const& p, DisulfideNC& disulfide,
            double& V, Vector& dV_dr1, Vector& dV_dr2) const;
    };

    inline void NativeContacts::perNormal(vl::Base const& p, NormalNC& normal,
        double& V, Vector& dV_dr1, Vector& dV_dr2) const {

        normal.ff.asForce(p.unit, p.norm, V, dV_dr1, dV_dr2);
    }

    inline void NativeContacts::perDisulfide(const vl::Base &p,
        DisulfideNC &disulfide, double &V, Vector &dV_dr1,
        Vector &dV_dr2) const {

        disulfideV.asForce(p.unit, p.norm, V, dV_dr1, dV_dr2);
    }
}