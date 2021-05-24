#pragma once
#include "GoBase.hpp"
#include "../../stats/Stats.hpp"

namespace mdk {
    class NormalNative: public GoBase {
    public:
        double depth = 1.0 * eps;

        std::vector<Target> sat() const override;
        void bind(Simulation& simulation) override;
        void run() override;

    private:
        vl::Spec spec() const override;
    };
}