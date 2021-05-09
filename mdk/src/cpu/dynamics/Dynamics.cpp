#include "cpu/dynamics/Dynamics.hpp"
using namespace mdk;

void Dynamics::reset() {
    V = 0.0;
    for (auto& f: F) f.setZero();
}