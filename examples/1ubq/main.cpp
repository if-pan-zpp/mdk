#include <mdk/simul/Simulation.hpp>
#include <mdk/files/pdb/Parser.hpp>
#include <mdk/hooks/ExportPDB.hpp>
#include <mdk/hooks/ProgressBar.hpp>
#include <mdk/files/param/LegacyParser.hpp>
#include <mdk/forces/All.hpp>
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

    auto rand = make_shared<Random>(0);
    auto density = 1e-4*atom/pow(angstrom, 3);
    auto ixDist = 4.56*angstrom;
    model.morphIntoSAW(*rand, false, density, ixDist);

    ofstream model0("data/model_0.pdb");
    pdb::Parser().write(model0, pdb::Data(pdb::Model(model)));

    Simulation simul(model, params);

    auto& lf = simul.add<Leapfrog>(0.002 * tau);
    auto& srand = simul.add<Random>(0);
    auto& lang = simul.add<LangevinDynamics>(2.0/tau, 300.0*Kelvin);
//    auto& teth = simul.add<Tether>(true);
//    auto& nba = simul.add<NativeBondAngle>();
//    auto& hba = simul.add<HeuresticBondAngle>();
//    auto& cnd = simul.add<ComplexNativeDihedral>();
//    auto& hd = simul.add<HeuresticDihedral>();
//    auto& nc = simul.add<NativeContacts>();
//    auto& cdh = simul.add<ConstDH>();
//    auto& qa = simul.add<QuasiAdiabatic>();
//    auto& pe = simul.add<PauliExclusion>();

    auto& exp = simul.add<ExportPDB>("model.pdb", 10.0 * nanosecond);
    auto time = 15000.0*tau;
    auto& prog = simul.add<ProgressBar>(time);

    auto& state = simul.var<State>();
    while (state.t < time)
        simul.step();

    return 0;
}