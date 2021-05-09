#pragma once
#include <mdk/tools/model/Model.hpp>
#include <mdk/cpu/generic/LennardJones.hpp>
#include <mdk/cpu/generic/DisulfideV.hpp>
#include <mdk/cpu/verlet/VL.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>
#include <vector>
#include <optional>

namespace mdk {
    struct NativeNormal {
        double r_min;
    };

    struct NativeDisulfide {};

    class NativeContacts {
    public:
        DisulfideV disulfideV;

        mutable double _cutoff;
        double cutoff() const;

        std::vector<std::pair<std::pair<int, int>, NativeNormal>> normals;
        std::vector<std::pair<std::pair<int, int>, NativeDisulfide>> disulfides;

        explicit NativeContacts(Model const& model,
            DisulfideV const& disulfideV);

        inline void perNormal(vl::Base const& p, NativeNormal& normal,
             Dynamics& dyn) const {

            thread_local LennardJones lj;
            lj.r_min = normal.r_min;
            lj.asForce(p.unit, p.norm, dyn.V, dyn.F[p.i1], dyn.F[p.i2]);
        }

        inline void perDisulfide(vl::Base const& p, Dynamics& dyn) const {
            disulfideV.asForce(p.unit, p.norm, dyn.V, dyn.F[p.i1], dyn.F[p.i2]);
        }
    };
}