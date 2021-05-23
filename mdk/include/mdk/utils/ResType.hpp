#pragma once
#include <vector>
#include <cstdint>
#include <string>

namespace mdk {
    enum class ResTypeIdx: int8_t {
        ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE,
        LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL
    };

    class ResType {
    public:
        ResType() = default;

        constexpr explicit ResType(ResTypeIdx code): code {code} {};
        explicit operator ResTypeIdx const&() const;

        constexpr ResType(int8_t x): ResType((ResTypeIdx)x) {};
        operator int8_t() const;

        explicit ResType(std::string const& name);
        explicit operator std::string() const;

        double mass() const;

    private:
        ResTypeIdx code;
        friend struct std::hash<ResType>;
    };
}

namespace std {
    template<>
    struct hash<mdk::ResType> {
        size_t operator()(mdk::ResType const &resType) const {
            return std::hash<mdk::ResTypeIdx>()(resType.code);
        }
    };
}