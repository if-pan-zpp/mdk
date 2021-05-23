#pragma once
#include <cpu/core/BaseState.hpp>
#include <forces/Force.hpp>

namespace mdk {
    class Chirality: public Force {
    private:
        Scalars d0_cube_inv, C_nat;
        std::vector<std::pair<int, int>> ranges;

    public:
        double e_chi = 1.0 * eps;
        explicit Chirality(Model const& model);

        void eval(BaseState const& state, BaseDiff& update) const override;
    };
}