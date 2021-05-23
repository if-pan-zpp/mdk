#pragma once
#include "../utils/Units.hpp"
#include "../system/State.hpp"
#include "../runtime/Task.hpp"
#include "../model/Model.hpp"
#include "../simul/BoundEntity.hpp"
#include <filesystem>

namespace mdk {
    class ExportPDB: public Task, BoundEntity {
    private:
        Model base;
        State *state = nullptr;

        std::filesystem::path modelPath;
        double period, tprev = 0.0;

    public:
        explicit ExportPDB(std::filesystem::path modelPath,
            double period = 1.0 * microsecond):
            modelPath(std::move(modelPath)),
            period(period) {};

        void bind(Simulation& simulation) override;
        std::vector<Target> req() const override;
        std::vector<Target> sat() const override;
        void run() override;
    };
}