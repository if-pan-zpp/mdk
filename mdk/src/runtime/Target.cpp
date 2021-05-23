#include "runtime/Target.hpp"
using namespace mdk;

Target Target::create() {
    static int globalUID = 1;
    Target target;
    target.uid = globalUID++;
    return target;
}

bool Target::operator==(const Target &other) const {
    return uid == other.uid;
}

bool Target::operator<(const Target &other) const {
    return uid < other.uid;
}