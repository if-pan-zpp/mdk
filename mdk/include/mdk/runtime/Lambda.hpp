#pragma once
#include "Task.hpp"

namespace mdk {
    template<typename Lam>
    class Lambda: public Task {
    public:
        Lambda(std::vector<Target> req, Lam f, std::vector<Target> sat):
            _req(std::move(req)), _f(std::move(f)), _sat(std::move(sat)) {};

        std::vector<Target> const& req() const override {
            return _req;
        }

        std::vector<Target> const& sat() const override {
            return _sat;
        }

        void run() override {
            _f();
        }

    private:
        std::vector<Target> _req, _sat;
        Lam _f;
    };
}