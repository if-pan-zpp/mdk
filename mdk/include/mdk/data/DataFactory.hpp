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
        Data const& data() const {
            auto idx = std::type_index(typeid(Data));
            if (savedData.find(idx) == savedData.end()) {
                auto _data = std::make_shared<Data>(create<Data>());
                savedData.insert(idx, _data);
            }
            return savedData.at(idx);
        }

    private:
        Model const *model;
        param::Parameters const *params;
        std::unordered_map<std::type_index, std::any> savedData;

        template<typename Data>
        Data create() const;
    };
}