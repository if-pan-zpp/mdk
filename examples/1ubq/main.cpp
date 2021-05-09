#include <mdk/cpu/sys/System.hpp>
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

    System sys(model);
    Masses masses(model);

    sys.leapfrog = Leapfrog(masses);
    sys.langDyn = LangevinDynamics(masses, rand, 2.0/tau, 300.0*Kelvin);
    sys.pauliExcl = PauliExclusion();
    sys.harm = HarmonicTethers(model, true);
    sys.nativeBA = NativeBondAngles(model);
    sys.compNativeDih = ComplexNativeDihedrals(model);
    sys.natCont = NativeContacts(model, DisulfideV());
    sys.constDH = ConstDH(Charges(model, params));

    sys.hooks.emplace_back(make_shared<ExportPDB>(model,
       10*nanosecond, "model.pdb"));

    auto simul = 15000.0*tau;
    sys.hooks.emplace_back(make_shared<ProgressBar>(simul));

    sys.step(simul);

    return 0;
}