#pragma once
#include "../runtime/Task.hpp"
#include "../system/State.hpp"
#include "../simul/BoundEntity.hpp"

namespace mdk {
    class Force: public Task, BoundEntity {
    protected:
        State *state = nullptr;

    public:
        std::vector<Target> req() const override;
        std::vector<Target> sat() const override;
        void bind(Simulation& simulation) override;
    };
}