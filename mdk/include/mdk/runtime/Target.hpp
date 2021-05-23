#pragma once
#include <unordered_map>

namespace mdk {
    class Target {
    public:
        Target();

        bool operator<(Target const& other) const;
        bool operator==(Target const& other) const;

    private:
        int uid;
        friend std::hash<Target>;
    };
}

namespace std {
    template<>
    struct hash<mdk::Target> {
        size_t operator()(mdk::Target const& target) const {
            return target.uid;
        }
    };
}