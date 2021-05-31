#include <mdk/files/seq/LegacyParser.hpp>
#include <mdk/simul/Simulation.hpp>
#include <mdk/files/param/LegacyParser.hpp>
#include <mdk/system/LangPredictorCorrector.hpp>
#include <mdk/forces/All.hpp>
#include <mdk/hooks/PositionDiff.hpp>
#include <fstream>
using namespace mdk;
using namespace std;

int main() {
    Model model = seq::LegacyParser().read("glut.txt").asModel();

    ifstream param_file("parameters.txt");
    auto params = param::LegacyParser().read(param_file);

    cout << model.n << endl;

    ifstream xyz("startconf.xyz", ifstream::in);
    for (int i = 0; i < model.n; ++i) {
        for (int dim = 0; dim < 3; ++dim) {
            xyz >> model.residues[i].r(dim);
        }
        model.residues[i].r *= angstrom;
        model.residues[i].nat_r = model.residues[i].r;
    }

    auto rand = Random(4359);
    rand.uniform();
    model.initVelocity(rand, 0.35 * eps_kB, false);

    Simulation simul(model, params);

    simul.add<Random>(rand);
    simul.add<LangPredictorCorrector>(0.005 * tau);

    simul.add<Tether>(true);
    simul.add<NativeBA>();
    simul.add<HeuresticBA>();
    simul.add<ComplexNativeDihedral>();
    simul.add<HeuresticDihedral>();

    simul.add<NativeContacts>();
    simul.add<PauliExclusion>();

    // simul.add<PositionDiff>(model, "positions.txt", "posDiff.out");
    // for (int i = 0; i < 200; ++i) {
    // 	simul.step();
    // }

    for (int i = 0; i < 200 * 20; ++i) {
    	simul.step();
    }
    return 0;
}
