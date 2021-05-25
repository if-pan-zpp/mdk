#pragma once
#include <memory>
#include "../runtime/Task.hpp"

namespace mdk::vl {
    class UpdateScheduler {
    public:
        std::vector<std::shared_ptr<Task>> destructiveUpdates;
        std::vector<std::shared_ptr<Task>> asyncUpdates;

        void run();
    };
}