#pragma once
#include <string>
#include <vector>

namespace mdk {
    class Field {
    public:
        virtual void read(const std::string &s) = 0;
        virtual void write(std::string &s) const = 0;
    };
}