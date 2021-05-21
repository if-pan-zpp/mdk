#include "cpu/core/Simulation.hpp"
#include "cpu/dyn/Integrator.hpp"
#include "cpu/forces/Force.hpp"
#include "cpu/hooks/Hook.hpp"
#include "cpu/core/Thread.hpp"
#include "cpu/dyn/PhysicalState.hpp"
using namespace mdk;
using namespace std;

Simulation::Simulation(const Model &model, const param::Parameters &params):
    df(model, params) {};

void Simulation::step(double t) {
    auto dt = integrator->stepTime();
    step((int)std::ceil(t / dt));
}

void Simulation::init() {

}

void Simulation::step(int n) {
    if (!initialized) init();

    auto& physicalState = state<PhysicalState>();
    unordered_map<type_index, std::vector<Diff*>> diffs;
    vector<Thread*> allThreads;

    for (auto& thr: threads) {
        for (auto& [diff_idx, diff]: thr.diffs) {
            diffs[diff_idx].emplace_back(diff.get());
        }
        allThreads.emplace_back(&thr);
    }

    vector<future<void>> futures;
    auto sync = [&]() -> void {
        for (auto& fut: futures)
            fut.wait();
    };

    for (int s = 0; s < n; ++s) {
        futures.clear();
        for (auto& hook: hooks) {
            futures.emplace_back(hook->exec(allThreads));
        }
        sync();

        for (auto& thr: threads) {
            for (auto& [key, diff]: thr.diffs) {
                diff->reset();
            }
        }

        futures.clear();
        for (auto const& force: forces) {
            futures.emplace_back(force->exec(allThreads));
        }
        sync();

        for (auto& [state_idx, state]: states) {

        }
    }
}