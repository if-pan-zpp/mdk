#pragma once
#include "Task.hpp"
#include "Target.hpp"
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>

namespace mdk {
    class TaskSet: public Task {
    public:
        std::vector<Target> const& req() const override;
        std::vector<Target> const& sat() const override;

        void compile(Target reach);
        void run() override;

        template<typename TaskImpl>
        TaskImpl& add(TaskImpl task) {
            auto& _task = std::make_unique<TaskImpl>(std::move(task));
            tasks.emplace_back(std::move(_task));
        }

    private:
        std::vector<std::unique_ptr<Task>> tasks;
        std::unordered_map<Target, std::vector<int>> taskReqs, taskSats;
        std::vector<std::mutex> mutexes;

        bool compiled;
    };
}