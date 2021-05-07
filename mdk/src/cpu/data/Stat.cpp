#include "cpu/data/Stat.hpp"
using namespace mdk;

Stat& Stat::operator+=(const Stat &other) {
    backbone += other.backbone;
    sidechain += other.sidechain;
    hydrophobicSS += other.hydrophobicSS;
    polarSS += other.polarSS;
    return *this;
}

Stat Stat::operator+(const Stat &other) const {
    auto res = *this;
    res += other;
    return res;
}

Stat Stat::operator-() const {
    auto res = *this;
    res.backbone = -res.backbone;
    res.sidechain = -res.sidechain;
    res.polarSS = -res.polarSS;
    res.hydrophobicSS = -res.hydrophobicSS;
    return res;
}

Stat& Stat::operator-=(const Stat &other) {
    *this += -other;
    return *this;
}

Stat Stat::operator-(const Stat &other) const {
    auto res = *this;
    res -= other;
    return res;
}