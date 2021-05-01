#pragma once
#include <mdk/cpu/ff/ForceField.hpp>

namespace mdk {
    class Chirality: public ForceField {
    private:
        Regions regions;
        Scalars d0_cube_invs, C_nats;

        double e_chi = 1.0 * eps;

    public:
        Chirality(Model const& model);
        void compute(State const& state, double& V, Vectors& dV_dr) override;
    };
}