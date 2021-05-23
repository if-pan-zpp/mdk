#pragma once
#include "../model/Model.hpp"
#include "../files/param/Parameters.hpp"
#include "../data/DataFactory.hpp"
#include <typeindex>
#include <any>

namespace mdk {
    class Simulation {
    public:
        Simulation(Model model, param::Parameters params):
            model(std::move(model)), params(std::move(params)),
            df(this->model, this->params) {};

        template<typename Data>
        Data data() {
            return df.data<Data>();
        }

        template<typename Var>
        Var& var() {
            auto& _var = vars[std::type_index(typeid(Var))];
            return static_cast<Var&>(_var);
        }

    private:
        Model model;
        param::Parameters params;
        DataFactory df;

        std::unordered_map<std::type_index, std::any> vars;
    };
}