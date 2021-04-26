#pragma once
#include <mdk/cpu/ff/local/LocalFF.hpp>
#include <mdk/cpu/core/Particles.hpp>
#include <mdk/utils/Units.hpp>

namespace mdk {
    class Chirality: public LocalFF<Chirality> {
    private:
        Particles const* pts;
        Scalars Cnat, d0CubeInv;

    public:
        double echi = 1.0*eps;

        explicit Chirality(Particles const& pts);
        void localTerm(double& V, Vectors& gradV, int i, int beg, int end);
    };
}