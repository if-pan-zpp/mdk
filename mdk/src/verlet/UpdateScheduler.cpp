#include "verlet/UpdateScheduler.hpp"
using namespace mdk::vl;

void UpdateScheduler::run() {
    for (auto& task: destructiveUpdates) {
        task->run();
    }

    for (auto& task: asyncUpdates) {
        task->run();
    }
}