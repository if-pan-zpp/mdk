#include "cpu/local/LocalExclusion.hpp"
using namespace mdk;

void LocalRepulsive::setCutoff(double cutoff) {
    this->cutoff = cutoff;
    cutoff2 = pow(cutoff, 2.0);
}