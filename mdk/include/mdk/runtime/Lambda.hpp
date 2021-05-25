#pragma once
#include "Task.hpp"

namespace mdk {
    template<typename Lam>
    class Lambda: public Task {
    public:
        Lambda(std::vector<Target*> req, Lam f, std::vector<Target*> sat):
            _req(std::move(req)), _sat(std::move(sat)), _f(std::move(f)) {};

        std::vector<Target*> req() override {
            return _req;
        }

        std::vector<Target*> sat() override {
            return _sat;
        }

        void run() override {
            _f();
        }

        std::unique_ptr<Task> unique() const {
            return std::make_unique<Lambda<Lam>>(*this);
        }

    private:
        std::vector<Target*> _req, _sat;
        Lam _f;
    };
}