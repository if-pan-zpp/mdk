#include "data/DataFactory.hpp"
#include "data/BondAngleRanges.hpp"
#include "data/Chains.hpp"
#include "data/Charges.hpp"
#include "data/DihedralRanges.hpp"
#include "data/Masses.hpp"
#include "data/Types.hpp"
using namespace mdk;

template<>
BondAngleRanges DataFactory::data<BondAngleRanges>() const {
    return BondAngleRanges(*model);
}

template<>
Chains DataFactory::data<Chains>() const {
    return Chains(*model);
}

template<>
Charges DataFactory::data<Charges>() const {
    return Charges(*model, *params);
}

template<>
DihedralRanges DataFactory::data<DihedralRanges>() const {
    return DihedralRanges(*model);
}

template<>
Masses DataFactory::data<Masses>() const {
    return Masses(*model);
}

template<>
Types DataFactory::data<Types>() const {
    return Types(*model);
}

template<>
Model DataFactory::data<Model>() const {
    return *model;
}

template<>
param::Parameters DataFactory::data<param::Parameters>() const {
    return *params;
}