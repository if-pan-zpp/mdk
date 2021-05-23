#include "cpu/forces/NonlocalForce.hpp"
using namespace mdk;

vl::Spec const& NonlocalForce::spec() const {
    _spec = recomputeSpec();
    return _spec;
}