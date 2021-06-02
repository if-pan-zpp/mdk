#pragma once
#include "../system/State.hpp"
#include "../utils/Units.hpp"
#include "../simul/SimulVar.hpp"
#include "../data/Chains.hpp"
#include "Spec.hpp"

namespace mdk {
    class NonlocalForce;
}

namespace mdk::vl {
    class List: public SimulVar {
    private:
        State const *state = nullptr;
        Chains const* chains = nullptr;
        double t0 = 0.0;
        Vectors r0;
        Topology top0;

        std::vector<NonlocalForce*> forces;

        bool initial = false;
        double cutoff = 0.0 * angstrom;
        double pad = 10.0 * angstrom;
        int minBondSep = 0;

        bool needToReset() const;
        void update();

        Eigen::Vector3i grid;
        std::vector<int> first, last, next;
        double effCutoff, effCutoffSq;
        int indexOf(Eigen::Vector3i const& loc);
        void perPair(int c1, int c2);
        void perCell(int c1);
        void updateGrid();

    public:
        void registerNF(NonlocalForce& force, Spec const& spec);

        Pairs pairs;
        void bind(Simulation& simulation) override;

        void check();
    };
}