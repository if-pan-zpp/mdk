#pragma once
#include "Task.hpp"
#include "Target.hpp"
#include <vector>
#include <memory>
#include <mutex>

namespace mdk {
    class Scheduler {
    public:
        void add(Task* task);
        void run();

    private:
        std::vector<Task*> tasks;

        bool compiled = false;
        void compile();
    };
}