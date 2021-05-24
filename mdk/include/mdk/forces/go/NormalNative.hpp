#pragma once
#include <forces/go/GoBase.hpp>

namespace mdk {
    class NormalNative: public GoBase {
    public:
        double depth = 1.0 * eps;

        void bind(Simulation& simulation) override;
        void run() override;

    protected:
        vl::Spec spec() const override;
    };
}