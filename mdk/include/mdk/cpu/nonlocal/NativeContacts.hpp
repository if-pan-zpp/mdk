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
        mutable double _cutoff;
        double cutoff() const;

        std::vector<std::pair<std::pair<int, int>, NativeContact>> normals;

        explicit NativeContacts(Model const& model);

        inline void perNormal(NativeContact const& x, Dynamics& dyn) const {
            thread_local LennardJones lj;
            lj.r_min = x.r_min;
            lj.asForce(x.unit, x.norm, dyn.V, dyn.F[x.i1], dyn.F[x.i2]);
        }
    };
}