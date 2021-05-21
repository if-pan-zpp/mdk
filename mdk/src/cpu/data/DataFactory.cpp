#include <cpu/data/DataFactory.hpp>
#include <cpu/data/BondAngleRanges.hpp>
#include <cpu/data/Chains.hpp>
#include <cpu/data/Charges.hpp>
#include <cpu/data/DihedralRanges.hpp>
#include <cpu/data/Masses.hpp>
#include <cpu/data/Types.hpp>

using namespace mdk;

DataFactory::DataFactory(Model model, param::Parameters params) {
    this->model = std::make_unique<Model>(std::move(model));
    this->params = std::make_unique<param::Parameters>(std::move(params));
}

template<>
std::shared_ptr<Model> DataFactory::data<Model>() {
    return model;
}

template<>
std::shared_ptr<param::Parameters> DataFactory::data<param::Parameters>() {
    return params;
}

template<>
std::shared_ptr<BondAngleRanges> DataFactory::data<BondAngleRanges>() {
    return std::make_shared<BondAngleRanges>(*model);
}

template<>
std::shared_ptr<Chains> DataFactory::data<Chains>() {
    return std::make_shared<Chains>(*model);
}

template<>
std::shared_ptr<Charges> DataFactory::data<Charges>() {
    return std::make_shared<Charges>(*model, *params);
}

template<>
std::shared_ptr<DihedralRanges> DataFactory::data<DihedralRanges>() {
    return std::make_shared<DihedralRanges>(*model);
}

template<>
std::shared_ptr<Masses> DataFactory::data<Masses>() {
    return std::make_shared<Masses>(*model);
}

template<>
std::shared_ptr<Types> DataFactory::data<Types>() {
    return std::make_shared<Types>(*model);
}
