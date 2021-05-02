#include <mdk/tools/seq/LegacyParser.hpp>
#include <mdk/tools/pdb/Parser.hpp>
#include <fstream>
using namespace mdk;

int main() {
    auto seq = seq::LegacyParser().read("data/glut.txt").asModel();

    auto rand = FortranRandom(0);
    auto density = 1e-4*atom/pow(angstrom, 3);
    auto ixDist = 4.56*angstrom;
    seq.morphIntoSAW(rand, false, density, ixDist);

    auto output = std::ofstream("data/glut.pdb");
    pdb::Parser().write(output, pdb::Data(AtomicModel(seq)));

    return 0;
}
