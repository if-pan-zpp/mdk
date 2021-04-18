#include <mdk/files/pdb/PDBParser.hpp>
#include <fstream>
using namespace std;
using namespace mdk;

int main() {
    ifstream ubq_pdb_file("data/1ubq.pdb");
    PDBParser parser;
    auto ubq_pdb = parser.read(ubq_pdb_file);

    ofstream ubq_save_file("data/1ubq.save.pdb");
    parser.write(ubq_save_file, ubq_pdb);

    return 0;
}