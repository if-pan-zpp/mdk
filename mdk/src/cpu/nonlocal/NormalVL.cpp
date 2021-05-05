#include "cpu/nonlocal/NormalVL.hpp"
#include "cpu/System.hpp"
using namespace mdk;

NormalVL::NormalVL(const System &system) {
    owner = &system;
}
