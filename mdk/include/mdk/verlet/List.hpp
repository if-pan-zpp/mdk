#pragma once
#include "../system/State.hpp"
#include "../utils/Units.hpp"
#include "../runtime/TaskFactory.hpp"

namespace mdk::vl {
    class List: public TaskFactory {
    private:
        State const* state0 = nullptr;
        double t0 = 0.0;
        Vectors r0;
        Topology top0;

        double cutoff = 18.0 * angstrom;
        double pad = 10.0 * angstrom;

        bool needToReset()

    public:
        List() = default;
        List(double cutoff, double pad):
            cutoff(cutoff), pad(pad) {};

        Pairs pairs;

        Target vlUpdated;
        std::vector<std::unique_ptr<Task>> tasks() override;
    };
}