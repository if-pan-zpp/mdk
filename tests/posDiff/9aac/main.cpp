#include <mdk/files/seq/LegacyParser.hpp>
#include <mdk/simul/Simulation.hpp>
#include <mdk/files/param/LegacyParser.hpp>
#include <mdk/hooks/PositionDiff.hpp>
#include <mdk/system/LangPredictorCorrector.hpp>
#include <mdk/forces/All.hpp>
#include <fstream>
using namespace mdk;
using namespace std;

int main() {
    auto model = seq::LegacyParser().read("data/9AAC.txt").asModel();

    ifstream param_file("data/parametersMD05.txt");
    auto params = param::LegacyParser().read(param_file);

    Random rand = Random(77977);
    rand.uniform();
    model.legacyMorphIntoSAW(rand, false, 0, 4.56*angstrom, false);
    model.initVelocity(rand, 0.35 * eps_kB, false);

    Simulation simul(model, params);

    simul.add<Random>(rand);
    simul.add<LangPredictorCorrector>(0.005 * tau);
    simul.add<PositionDiff>(model, "data/positions.txt", "posDiffs.out");

    simul.add<Tether>(false);
    simul.add<PauliExclusion>();

    simul.add<NativeBA>();
    simul.add<HeuresticBA>();
    simul.add<ComplexNativeDihedral>();
    simul.add<HeuresticDihedral>();
    
    simul.add<PseudoImproperDihedral>();

    for (int i = 0; i < 200; ++i) {
    	simul.step();
    }
    return 0;
}
