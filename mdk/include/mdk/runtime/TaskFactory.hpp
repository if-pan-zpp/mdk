#pragma once
#include <memory>
#include "Task.hpp"

namespace mdk {
    class TaskFactory {
    public:
        virtual std::vector<std::unique_ptr<Task>> tasks() = 0;
    };
}