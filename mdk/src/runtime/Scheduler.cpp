#include "runtime/Scheduler.hpp"
#include <unordered_map>
using namespace mdk;

void Scheduler::add(Task *task) {
    tasks.emplace_back(task);
    compiled = false;
}

void Scheduler::compile() {
    std::vector<int> reqLeft(tasks.size());
    std::vector<Task*> sorted;
    std::unordered_map<Target*, std::vector<int>> reqToTasks;
    std::unordered_map<Target*, int> satLeft;
    std::vector<int> withoutReq;

    for (int i = 0; i < (int)tasks.size(); ++i) {
        auto& _reqLeft = reqLeft[i];
        auto& task = tasks[i];

        for (auto& _req: task->req()) {
            ++_reqLeft;
            reqToTasks[_req].push_back(i);
        }

        for (auto& _sat: task->sat()) {
            ++satLeft[_sat];
        }

        if (_reqLeft == 0)
            withoutReq.push_back(i);
    }

    while (!withoutReq.empty()) {
        auto cur = withoutReq.back();
        withoutReq.pop_back();
        sorted.push_back(tasks[cur]);

        auto& task = tasks[cur];
        for (auto& _sat: task->sat()) {
            if (--satLeft[_sat] == 0) {
                for (auto& other: reqToTasks[_sat]) {
                    if (--reqLeft[other] == 0)
                        withoutReq.push_back(other);
                }
            }
        }
    }

    if (sorted.size() != tasks.size())
        throw;

    compiled = true;
}

void Scheduler::run() {
    if (!compiled)
        compile();

    for (auto& task: tasks)
        task->run();
}
