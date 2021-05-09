#pragma once
#include <mdk/cpu/hooks/Hook.hpp>
#include <mdk/utils/Units.hpp>
#include <mdk/cpu/data/State.hpp>
#include <string>

namespace mdk {
    class ExportPDB: public Hook {
    private:
        int counter = 0;
        double period, tprev = 0.0;
        std::string name;
        Model model;

    public:
        explicit ExportPDB(Model model, double period = 1.0 * microsecond,
            std::string name = "model"): period {period},
            name {std::move(name)}, model {std::move(model)} {};

        void execute(System& system) override;
    };
}