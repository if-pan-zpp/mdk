#include <mdk/files/pdb/Parser.hpp>
#include <fstream>
using namespace std;
using namespace mdk;

int main() {
    ifstream ubq_pdb_file("data/1ubq.pdb");
    pdb::Parser parser;
    auto ubq = parser.read(ubq_pdb_file);

    ofstream ubq_save_file("data/1ubq.save.pdb");
    parser.write(ubq_save_file, ubq);

    return 0;
}