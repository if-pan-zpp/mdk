#pragma once
#include <mdk/cpu/data/Primitives.hpp>
#include <mdk/utils/Topology.hpp>
#include <mdk/cpu/forces/qa/QAState.hpp>

namespace mdk {
    class State {
    public:
        int n = 0;
        double t = 0.0;
        Vectors r, v;
        Topology top;

        QAState qaState;
    };
}