#pragma once
#include <vector>
#include <cstdint>
#include <string>

namespace mdk {
    enum class RTCode: int8_t {
        ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE,
        LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL
    };

    class ResType {
    public:
        ResType() = default;

        constexpr explicit ResType(RTCode code): code {code} {};
        explicit operator RTCode const&() const;

        constexpr explicit ResType(int8_t x): ResType((RTCode)x) {};
        explicit operator int8_t() const;

        explicit ResType(std::string const& name);
        explicit operator std::string() const;

    private:
        RTCode code;
        friend struct std::hash<ResType>;
    };
}

namespace std {
    template<>
    struct hash<mdk::ResType> {
        size_t operator()(mdk::ResType const &resType) const {
            return std::hash<mdk::RTCode>()(resType.code);
        }
    };
}