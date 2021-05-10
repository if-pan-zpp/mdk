#pragma once
#include <mdk/cpu/simul/State.hpp>

namespace mdk::vl {
    class Pairs {
    private:
        State const* state0 = nullptr;
        double t0;
        Vectors r0;
        Topology top0;

        Eigen::Vector3i grid;
        std::vector<int> first, next, last;

        int index(Eigen::Vector3i const& loc) const;

    private:
        void prepCells();
        void cellLoop();
        void neighLoop(int idx1, Eigen::Vector3i const& loc1);
        void forCellPair(int idx1, int idx2);
        void genPairList();

    public:
        double cutoff = 0.0, pad = 5.0*angstrom;
        std::vector<std::pair<int, int>> pairs;

        bool needToReset(State const& state) const;
        bool update(State const& state);
    };
}