#include <cpu/forces/dihedral/SimpleNativeDihedral.hpp>
using namespace mdk;

void SimpleNativeDihedral::dihTerm(int i, double phi, double &V,
    double &dV_dphi) const  {

    auto diff = phi - phi0[i];
    V += 0.5 * CDH * diff * diff;
    dV_dphi += CDH * diff;
}