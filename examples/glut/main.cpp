#include <mdk/files/seq/LegacyParser.hpp>
#include <mdk/files/pdb/Parser.hpp>
#include <fstream>
using namespace mdk;
using namespace std;

int main() {
    auto model = seq::LegacyParser().read("data/glut.txt").asModel();

    auto rand = Random(0);
    auto density = 1e-4*atom/pow(angstrom, 3);
    auto ixDist = 4.56*angstrom;
    model.morphIntoSAW(rand, false, density, ixDist);

    ofstream coarseFile("data/glut.pdb");
    pdb::Parser().write(coarseFile, pdb::Data(pdb::Model(model)));
    return 0;
}
