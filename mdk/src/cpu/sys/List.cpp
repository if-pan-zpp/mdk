#include "cpu/sys/VL.hpp"
#include "cpu/sys/System.hpp"
using namespace mdk;

SystemVL::SystemVL(const System &sys) {
    this->owner = &sys;
}

