#pragma once
#include "../utils/Units.hpp"
#include "../system/State.hpp"
#include "../model/Model.hpp"
#include "../simul/SimulVar.hpp"
#include "../files/pdb/Data.hpp"
#include "Hook.hpp"
#include <filesystem>
#include <limits>

namespace mdk {
    class ExportPDB: public Hook, SimulVar {
    private:
        Model base;
        pdb::Data data;
        int modelIdx = 1;

        State *state = nullptr;

        std::filesystem::path modelPath;
        double period, tprev = std::numeric_limits<double>::lowest();

    public:
        explicit ExportPDB(std::filesystem::path modelPath,
            double period = 1000.0 * microsecond):
            modelPath(std::move(modelPath)),
            period(period) {};

        ~ExportPDB();

        void bind(Simulation& simulation) override;
        void execute(int step_nr) override;
    };
}
