#include "cpu/hooks/ExportPDB.hpp"
#include "cpu/sys/System.hpp"
#include "tools/pdb/Data.hpp"
#include "tools/model/AtomicModel.hpp"
#include "tools/pdb/Parser.hpp"
#include <fstream>
using namespace mdk;

void ExportPDB::execute(System &system) {
    if (system.state.t - tprev >= period) {
        system.state.saveTo(model);

        auto modelFile = std::ofstream(name);
        pdb::Parser().write(modelFile, pdb::Data(AtomicModel(model)));

        tprev = system.state.t;
    }
}