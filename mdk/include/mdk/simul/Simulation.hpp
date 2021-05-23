#pragma once
#include "../model/Model.hpp"
#include "../files/param/Parameters.hpp"

namespace mdk {
    class Simulation {
    public:
        Simulation(Model model, param::Parameters params):
            model(std::move(model)), params(std::move(params)) {};

    private:
        Model model;
        param::Parameters params;
    };
}