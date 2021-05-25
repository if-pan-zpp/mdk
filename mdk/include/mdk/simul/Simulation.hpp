#pragma once
#include "../model/Model.hpp"
#include "../files/param/Parameters.hpp"
#include "../data/DataFactory.hpp"
#include "../runtime/Scheduler.hpp"
#include "../runtime/TaskFactory.hpp"
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

        template<typename T, typename... Args>
        T& add(Args&&... args) {
            auto& _var = vars[std::type_index(typeid(T))];
            _var = T(std::forward<Args>(args)...);
            auto& x = std::any_cast<T&>(_var);

            if constexpr (std::is_base_of_v<SimulVar, T>) {
                ((SimulVar&)x).bind(*this);
            }

            if constexpr (std::is_base_of_v<Task, T>) {
                sched.add(&x);
            }

            if constexpr (std::is_base_of_v<TaskFactory, T>) {
                auto tasks = ((TaskFactory&)x).tasks();
                for (auto& task: tasks) {
                    sched.add(task.get());
                }

                extraTasks.insert(extraTasks.end(),
                    std::make_move_iterator(tasks.begin()),
                    std::make_move_iterator(tasks.end()));
            }

            return x;
        }

        void step();

    private:
        Model model;
        param::Parameters params;
        DataFactory df;
        Scheduler sched;

        std::unordered_map<std::type_index, std::any> vars;
        std::vector<std::unique_ptr<Task>> extraTasks;
    };
}