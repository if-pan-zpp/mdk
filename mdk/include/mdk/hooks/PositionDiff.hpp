#pragma once
#include "Hook.hpp"
#include "../simul/Simulation.hpp"
#include "../system/State.hpp"
#include "../model/Model.hpp"
#include <fstream>
#include <string>
#include <map>

namespace mdk {
    class PositionDiff : public Hook, SimulVar {
    public:
        PositionDiff(Model const& model, std::string inputPath, std::string outputPath);
        void bind(Simulation& simulation) override;
        void execute(int step_nr) override;
    private:
        std::ifstream input;
        std::ofstream output;
        std::map<int, Vectors> refPositions;

        State const* state;
    };
}
