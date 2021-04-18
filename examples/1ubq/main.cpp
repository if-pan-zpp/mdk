#include <mdk/files/pdb/Parser.hpp>
#include <mdk/files/param/LegacyParser.hpp>
#include <fstream>
using namespace std;
using namespace mdk;

int main() {
    ifstream param_file("data/parametersMJ96.txt");
    auto params = param::LegacyParser().read(param_file);

    return 0;
}