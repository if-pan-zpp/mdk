#include <mdk/tools/seq/LegacyParser.hpp>
#include <mdk/tools/pdb/Parser.hpp>
#include <fstream>
using namespace mdk;
using namespace std;

int main() {
    auto model = seq::LegacyParser().read("data/9AAC.txt").asModel();

    auto rand = FortranRandom(0);
    model.morphIntoSAW(rand, false);

    ofstream coarseFile("data/9AAC.pdb");
    pdb::Parser().write(coarseFile, pdb::Data(AtomicModel(model)));

    return 0;
}