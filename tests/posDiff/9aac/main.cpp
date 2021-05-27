#include <mdk/files/seq/LegacyParser.hpp>
#include <mdk/simul/Simulation.hpp>
#include <mdk/files/param/LegacyParser.hpp>
#include <mdk/hooks/PositionDiff.hpp>
#include <mdk/system/Leapfrog.hpp>
#include <fstream>
using namespace mdk;
using namespace std;

int main() {
    auto model = seq::LegacyParser().read("data/9AAC.txt").asModel();

    ifstream param_file("data/parametersMJ05.txt");
    auto params = param::LegacyParser().read(param_file);

    auto rand = make_shared<Random>(77977);
    rand -> uniform();
    model.legacyMorphIntoSAW(*rand, false, 0, 4.56*angstrom, false);

    Simulation simul(model, params);

    simul.add<Leapfrog>(0.002 * tau);
    simul.add<PositionDiff>(model, "data/positions.txt", "posDiffs.out");

    for (int i = 0; i < 100; ++i) {
    	simul.step();
    }
    return 0;
}
