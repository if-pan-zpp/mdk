#include <mdk/tools/seq/LegacyParser.hpp>
using namespace mdk;

int main() {
    auto seq = seq::LegacyParser().read("data/glut.txt").asModel();
    return 0;
}
