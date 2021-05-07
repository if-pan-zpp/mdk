#pragma once
#include <mdk/cpu/data/State.hpp>

namespace mdk::vl {
    class Pairs {
    private:
        State const* state0 = nullptr;
        Vectors r0;
        Topology top0;

        double pad = 0.0;
        Eigen::Vector3i grid;
        std::vector<int> first, next, last;

        int index(Eigen::Vector3i const& loc) const;

    private:
        void prepCells();
        void cellLoop();
        void neighLoop(int idx1, Eigen::Vector3i const& loc1);
        void forCellPair(int idx1, int idx2);
        void genPairList();
        bool needToReset() const;

    public:
        double cutoff = 0.0;
        std::vector<std::pair<int, int>> pairs;

        bool update(State const& state);
    };
}