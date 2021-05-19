#include <mdk/cpu/simul/Simulation.hpp>
#include <mdk/tools/pdb/Parser.hpp>
#include <mdk/cpu/hooks/ExportPDB.hpp>
#include <mdk/cpu/hooks/ProgressBar.hpp>
#include <mdk/tools/param/LegacyParser.hpp>
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

    auto rand = make_shared<FortranRandom>(0);
    auto density = 1e-4*atom/pow(angstrom, 3);
    auto ixDist = 4.56*angstrom;
    model.morphIntoSAW(*rand, false, density, ixDist);

    ofstream model0("data/model_0.pdb");
    pdb::Parser().write(model0, pdb::Data(AtomicModel(model)));

    Simulation simul(model);
    Masses masses(model);

    simul.leapfrog = Leapfrog(masses);
    simul.langDyn = LangevinDynamics(masses, rand, 2.0/tau, 300.0*Kelvin);
    simul.harm = HarmonicTethers(model, true);
    simul.nativeBA = NativeBondAngles(model);
    simul.compNativeDih = ComplexNativeDihedrals(model);
    simul.natCont = NativeContacts(model);
//    simul.constDH = ConstDH(Charges(model, params));
//    simul.quasiAd = QuasiAdiabatic(model, params);
    simul.pauliExcl = PauliExclusion();

    simul.hooks.emplace_back(make_shared<ExportPDB>(model,
       10*nanosecond, "model.pdb"));

    auto time = 15000.0*tau;
    simul.hooks.emplace_back(make_shared<ProgressBar>(time));

    simul.step(time);

    return 0;
}