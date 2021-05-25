#pragma once
#include "../model/Model.hpp"
#include "../files/param/Parameters.hpp"
#include "../data/DataFactory.hpp"
#include "SimulVar.hpp"
#include <typeindex>
#include <any>
#include <type_traits>

namespace mdk {
    class Simulation {
    public:
        Simulation(Model model, param::Parameters params):
            model(std::move(model)), params(std::move(params)),
            df(this->model, this->params) {};

        template<typename Data>
        Data const& data() {
            return df.data<Data>();
        }

        template<typename Var>
        Var& var() {
            auto idx = std::type_index(typeid(Var));
            if (vars.find(idx) == vars.end()) {
                return add<Var>();
            }
            else {
                auto& _var = vars.at(idx);
                return std::any_cast<Var&>(_var);
            }
        }

        template<typename Entity>
        Entity& add() {
            // TODO: write this part super-generically
        }

    private:
        Model model;
        param::Parameters params;
        DataFactory df;

        std::unordered_map<std::type_index, std::any> vars;
    };
}