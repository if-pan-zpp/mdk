#include "runtime/Target.hpp"
using namespace mdk;

Target::Target() {
    static int globalUID = 1;
    uid = globalUID++;
}

bool Target::operator==(const Target &other) const {
    return uid == other.uid;
}

bool Target::operator<(const Target &other) const {
    return uid < other.uid;
}