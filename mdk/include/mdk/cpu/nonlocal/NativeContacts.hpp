#pragma once
#include <vector>
#include <optional>
#include <mdk/tools/model/Model.hpp>
#include <mdk/cpu/kernels/LennardJones.hpp>
#include <mdk/cpu/verlet/Base.hpp>
#include <mdk/cpu/dynamics/Dynamics.hpp>

namespace mdk {
    struct NativeContact: vl::Base {
        double r_min = 0.0;
    };

    class NativeContacts {
    public:
        explicit NativeContacts(Model const& model);
        std::vector<std::pair<std::pair<int, int>, NativeContact>> normals;

        double cutoff() const;

        inline void perNormal(NativeContact const& x, Dynamics& dyn) const {
            auto lj = LennardJones(x.r_min, 1.0 * eps);
            lj.asForce(x.unit, x.norm, dyn.V, dyn.F[x.i1], dyn.F[x.i2]);
        }
    };
}