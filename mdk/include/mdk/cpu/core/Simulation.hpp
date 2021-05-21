#pragma once
#include <mdk/cpu/core/Diff.hpp>
#include <mdk/cpu/core/State.hpp>
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>
#include <mdk/cpu/data/DataFactory.hpp>
#include <mdk/cpu/verlet/Factory.hpp>
#include <mdk/cpu/forces/NonlocalForce.hpp>
#include <mdk/cpu/core/Thread.hpp>

#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <any>

namespace mdk {
    class Integrator;
    class Force;
    class Hook;

    class Simulation {
    private:
        std::vector<Thread> threads;
        DataFactory df;
        vl::Factory vf;

        std::unique_ptr<Integrator> integrator;
        std::vector<std::unique_ptr<Force>> forces;
        std::vector<std::unique_ptr<NonlocalForce>> nonlocalForces;
        std::vector<std::unique_ptr<Hook>> hooks;

        bool initialized = false;
        void init();

    public:
        Simulation(Model const& model, param::Parameters const& params);

        template<typename Impl, typename... Args>
        Impl& setIntegrator(Args&&... args) {
            integrator = std::make_unique<Impl>(std::forward<Args>(args)...);
            return *integrator;
        }

        template<typename Impl, typename... Args>
        Impl& addForce(Args&&... args) {
            auto force = std::make_unique<Impl>(std::forward<Args>(args)...);
            return *forces.emplace_back(std::move(force));
        }

        template<typename Impl, typename... Args>
        Impl& addNonlocalForce(Args&&... args) {
            auto force = std::make_unique<Impl>(std::forward<Args>(args)...);
            return *nonlocalForces.emplace_back(std::move(force));
        }

        template<typename Impl, typename... Args>
        Impl& addHook(Args&&... args) {
            auto hook = std::make_unique<Impl>(std::forward<Args>(args)...);
            return *hooks.emplace_back(std::move(hook));
        }

        template<typename Data>
        std::shared_ptr<Data> const& data() {
            return df.data<Data>();
        }

        void step(double t);
        void step(int n = 1);

        State state;
    };
}