#include "hooks/ExportPDB.hpp"
#include "simul/Simulation.hpp"
#include "files/pdb/Data.hpp"
#include "files/pdb/Model.hpp"
#include "files/pdb/Parser.hpp"
#include <fstream>

using namespace mdk;

void mdk::ExportPDB::bind(Simulation &simulation) {
    base = simulation.data<Model>();
    state = &simulation.var<State>();
}

std::vector<Target*> ExportPDB::sat() {
    return { &state->beforeUpdate };
}

void ExportPDB::run() {
    if (state->t - tprev >= period) {
        state->exportTo(base);

        auto modelFile = std::ofstream(modelPath);
        pdb::Parser().write(modelFile, pdb::Data(pdb::Model(base)));

        tprev = state->t;
    }
}
