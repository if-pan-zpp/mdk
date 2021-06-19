#pragma once
#include "../simul/SimulVar.hpp"
#include "../simul/Simulation.hpp"
#include "State.hpp"

namespace mdk {
    /**
     * An integrator object; after the forces have been computed, it
     * takes the forces and adjusts the positions, velocities etc.
     */
    class Integrator: public SimulVar {
    protected:
        State *state = nullptr;

    public:
        void bind(Simulation& simulation) override;

        /**
         * Initialize the state of the integrator.
         * TODO: perhaps it should be just moved to \p bind.
         */
        virtual void init() = 0;

        virtual void integrate() = 0;
    };
}