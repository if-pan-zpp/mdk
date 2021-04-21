#include <mdk/files/pdb/Parser.hpp>
#include <fstream>
using namespace mdk;
using namespace std;

int main() {
    ifstream pdb_file("data/1ubq.pdb");
    auto model = pdb::Parser().read(pdb_file);

    return 0;
}