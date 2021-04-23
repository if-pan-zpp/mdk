#include <mdk/tools/pdb/Parser.hpp>
#include <fstream>
using namespace mdk;
using namespace std;

int main() {
    ifstream pdb_file("data/1ubq.pdb");
    auto model = pdb::Parser().read(pdb_file).asModel();
    model.addNativeContacts();

    auto coarsened = model.coarsen();
    coarsened.morphIntoLine();
    ofstream coarse_file("data/1ubq.line.pdb");
    pdb::Parser().write(coarse_file, pdb::Data(AtomicModel(coarsened)));

    return 0;
}