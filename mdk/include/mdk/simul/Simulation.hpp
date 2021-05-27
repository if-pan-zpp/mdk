#pragma once
#include "../model/Model.hpp"
#include "../files/param/Parameters.hpp"
#include "../data/DataFactory.hpp"
#include "SimulVar.hpp"
#include <typeindex>
#include <type_traits>
#include <stdexcept>

namespace mdk {
    class Force;
    class NonlocalForce;
    class Hook;
    class Integrator;

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
                if constexpr (std::is_default_constructible_v<Var>) {
                    return add<Var>();
                }
                else {
                    throw std::runtime_error("cannot construct Var");
                }
            }
            else {
                auto& _var = vars.at(idx);
                return *(Var*)_var.get();
            }
        }

        template<typename T, typename... Args>
        T& add(Args&&... args) {
            auto _var = std::make_shared<T>(std::forward<Args>(args)...);
            auto idx = std::type_index(typeid(T));
            auto varIter = vars.emplace(idx, _var).first;
            auto& x = *(T*)varIter->second.get();

            if constexpr (std::is_base_of_v<SimulVar, T>) {
                ((SimulVar&)x).bind(*this);
            }

            if constexpr (std::is_base_of_v<Force, T>) {
                forces.push_back(&(Force&)x);
            }

            if constexpr (std::is_base_of_v<NonlocalForce, T>) {
                nonlocalForces.push_back(&(NonlocalForce&)x);
            }

            if constexpr (std::is_base_of_v<Hook, T>) {
                hooks.push_back(&(Hook&)x);
            }

            if constexpr (std::is_base_of_v<Integrator, T>) {
                integrator = &(Integrator&)x;
            }

            return x;
        }

        void step();

    private:
        Model model;
        param::Parameters params;
        DataFactory df;

        std::unordered_map<std::type_index, std::shared_ptr<void>> vars;
        std::vector<Force*> forces;
        std::vector<NonlocalForce*> nonlocalForces;
        std::vector<Hook*> hooks;
        Integrator *integrator;
    };
}