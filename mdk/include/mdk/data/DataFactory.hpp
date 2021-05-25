#pragma once
#include "../model/Model.hpp"
#include "../files/param/Parameters.hpp"
#include <memory>
#include <typeindex>
#include <any>
#include <unordered_map>

namespace mdk {
    class DataFactory {
    public:
        DataFactory(Model const& model, param::Parameters const& params):
            model(&model), params(&params) {};

        template<typename Data>
        Data const& data() {
            auto idx = std::type_index(typeid(Data));
            if (savedData.find(idx) == savedData.end()) {
                savedData.emplace(idx, create<Data>());
            }
            return std::any_cast<Data>(savedData.at(idx));
        }

    private:
        Model const *model;
        param::Parameters const *params;
        std::unordered_map<std::type_index, std::any> savedData;

        template<typename Data>
        Data create() const;
    };
}