#pragma once
#include "../system/State.hpp"
#include "../utils/Units.hpp"
#include "../runtime/TaskFactory.hpp"
#include "../simul/SimulVar.hpp"
#include "../data/Chains.hpp"
#include "UpdateScheduler.hpp"
#include "Spec.hpp"

namespace mdk::vl {
    class List: public TaskFactory, SimulVar {
    private:
        State const *state = nullptr;
        Chains const* chains = nullptr;
        double t0 = 0.0;
        Vectors r0;
        Topology top0;

        double cutoff = 0.0 * angstrom;
        double pad = 10.0 * angstrom;
        int minBondSep = 0;

        bool needToReset() const;
        void update();

    public:
        void registerSpec(Spec const& spec);

        Pairs pairs;
        void bind(Simulation& simulation) override;

        Target vlChecked;
        std::vector<std::unique_ptr<Task>> tasks() override;
        UpdateScheduler updateScheduler;
    };
}