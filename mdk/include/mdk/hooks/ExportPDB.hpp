#pragma once
#include "../utils/Units.hpp"
#include "../system/State.hpp"
#include "../model/Model.hpp"
#include "../simul/SimulVar.hpp"
#include "Hook.hpp"
#include <filesystem>

namespace mdk {
    class ExportPDB: public Hook, SimulVar {
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
        void execute() override;
    };
}