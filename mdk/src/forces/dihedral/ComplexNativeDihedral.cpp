#include "forces/dihedral/ComplexNativeDihedral.hpp"
using namespace mdk;

void ComplexNativeDihedral::dihTerm(int i, double phi, double &V,
    double &dV_dphi) const  {

    double _phi0 = phi0[i];

    V += CDA * (1.0 - cos(phi - _phi0)) +
         CDB * (1.0 - cos(3.0 * (phi - _phi0)));

    dV_dphi += CDA * sin(phi - _phi0) +
               3.0 * CDB * sin(3.0 * (phi - _phi0));
}