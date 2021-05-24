#pragma once
#include <cstdint>

namespace mdk {
    struct Stat {
        int8_t backbone = 0;
        int8_t sidechain = 0;
        int8_t hydrophobicSS = 0;
        int8_t polarSS = 0;

        inline bool valid() const {
            return backbone >= 0 && sidechain >= 0 &&
                   hydrophobicSS >= 0 && polarSS >= 0;
        }

        inline Stat& operator+=(Stat const& other) {
            backbone += other.backbone;
            sidechain += other.sidechain;
            hydrophobicSS += other.hydrophobicSS;
            polarSS += other.polarSS;
            return *this;
        }

        inline Stat operator+(Stat const& other) const {
            auto res = *this;
            res += other;
            return res;
        }

        inline Stat operator-() const {
            auto res = *this;
            res.backbone = -res.backbone;
            res.sidechain = -res.sidechain;
            res.polarSS = -res.polarSS;
            res.hydrophobicSS = -res.hydrophobicSS;
            return res;
        }

        inline Stat& operator-=(Stat const& other) {
            *this += -other;
            return *this;
        }

        inline Stat operator-(Stat const& other) const {
            auto res = *this;
            res -= other;
            return res;
        }
    };
}