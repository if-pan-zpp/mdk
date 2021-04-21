#include <mdk/files/pdb/Parser.hpp>
#include <fstream>
using namespace mdk;
using namespace std;

int main() {
    ifstream pdb_file("data/1ubq.pdb");
    auto pdb = pdb::Parser().read(pdb_file);

    ofstream pdb_save_file("data/1ubq.save.pdb");
    pdb::Parser().write(pdb_save_file, pdb);

    return 0;
}