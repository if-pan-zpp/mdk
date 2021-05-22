#pragma once
#include <mdk/cpu/runtime/Task.hpp>
#include <mdk/cpu/runtime/Target.hpp>
#include <unordered_map>

namespace mdk {
    class TaskSet: public Task {
    public:
        std::vector<Target> prerequisites() const override;
        std::vector<Target> satisfied() const override;
        std::future<void> perform() override;

        void add(Task const& task);

    private:

    };
}