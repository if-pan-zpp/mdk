#pragma once
#include <mdk/cpu/dyn/PhysicalState.hpp>

namespace mdk::vl {
    class Factory {
    private:
        PhysicalState const* state0 = nullptr;
        double t0 = 0.0;
        Vectors r0;
        Topology top0;

        Eigen::Vector3i grid;
        std::vector<int> first, next, last;

        double cutoff = 18.0*angstrom, pad = 10.0*angstrom;
        int index(Eigen::Vector3i const& loc) const;

    private:
        void prepCells();
        void cellLoop();
        void neighLoop(int idx1, Eigen::Vector3i const& loc1);
        void forCellPair(int idx1, int idx2);
        void genPairList();

    public:
        Factory() = default;
        Factory(double cutoff, double pad):
            cutoff(cutoff), pad(pad) {};

        Pairs pairs;

        bool needToReset(PhysicalState const& state) const;
        bool update(PhysicalState const& state);
    };
}