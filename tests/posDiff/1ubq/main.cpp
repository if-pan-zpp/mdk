#include <mdk/simul/Simulation.hpp>
#include <mdk/files/pdb/Parser.hpp>
#include <mdk/files/param/LegacyParser.hpp>
#include <mdk/hooks/PositionDiff.hpp>
#include <mdk/system/Leapfrog.hpp>
#include <fstream>
using namespace mdk;
using namespace std;

int main() {
    ifstream pdb_file("data/1ubq.pdb");
    auto atomic = pdb::Parser().read(pdb_file).asModel();

    ifstream param_file("data/parametersMJ96.txt");
    auto params = param::LegacyParser().read(param_file);

    atomic.addContactsFromAtomOverlap();
    auto model = atomic.coarsen();

    auto rand = make_shared<Random>(448);
    rand -> uniform();

    model.legacyMorphIntoSAW(*rand, false, 0, 4.56*angstrom, true);

    Simulation simul(model, params);

    simul.add<Leapfrog>(0.002 * tau);
    simul.add<PositionDiff>(model, "data/positions.txt", "posDiffs.out");

    for (int i = 0; i < 100; ++i) {
    	simul.step();
    }
    return 0;
}
