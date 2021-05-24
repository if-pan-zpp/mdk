#pragma once
#include "Stat.hpp"
#include "../simul/BoundEntity.hpp"
#include <vector>
#include "../runtime/TaskFactory.hpp"

namespace mdk {
    struct StatDiff {
        int i1, i2;
        Stat diffs[2];
        enum class Status: int8_t { CREATED, ACCEPTED, REJECTED } status;
    };

    class Stats: public BoundEntity, TaskFactory {
    public:
        std::vector<Stat> stats;
        bool used = false;

        std::vector<StatDiff> statDiffs;

        Target diffsReset = Target::create();
        Target diffsAdded = Target::create();
        Target statsUpdated = Target::create();

        void bind(Simulation& simulation) override;
        std::vector<std::unique_ptr<Task>> tasks() override;
    };
}