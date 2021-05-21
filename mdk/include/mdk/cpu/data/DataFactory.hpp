#pragma once
#include <mdk/tools/model/Model.hpp>
#include <mdk/tools/param/Parameters.hpp>

namespace mdk {
    class DataFactory {
    private:
        std::shared_ptr<Model> model;
        std::shared_ptr<param::Parameters> params;

    public:
        DataFactory(Model model, param::Parameters params);

        template<typename Data>
        std::shared_ptr<Data> data();
    };
}