#pragma once
#include "../model/Model.hpp"
#include "../files/param/Parameters.hpp"

namespace mdk {
    class DataFactory {
    public:
        DataFactory(Model const& model, param::Parameters const& params):
            model(&model), params(&params) {};

        template<typename Data>
        Data data() const;

    private:
        Model const *model;
        param::Parameters const *params;
    };
}