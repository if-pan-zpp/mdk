#pragma once
#include "../system/State.hpp"
#include "../simul/SimulVar.hpp"
#include "../simul/Simulation.hpp"

namespace mdk {
    class Force: public SimulVar {
    protected:
        State const* state = nullptr;

    public:
        void bind(Simulation& simulation) override;
        virtual void asyncPart(Dynamics &);
        virtual void syncPart(Dynamics &);
        virtual ~Force() {}
    };
}
