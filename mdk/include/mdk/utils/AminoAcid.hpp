#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace mdk {
    enum class AminoAcidIdx: int8_t {
        ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE,
        LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL
    };

    struct AAAtomInfo {
        double radius;
        bool inBackbone = false;
    };

    struct AminoAcidInfo {
        double mass;
        std::unordered_set<std::string> heavyAtoms;
        std::unordered_map<std::string, AAAtomInfo> atomInfo;
    };

    class AminoAcid {
    public:
        AminoAcid() = default;
        static std::vector<AminoAcid> aminoAcids();

        static std::vector<AminoAcidIdx> types();
        constexpr explicit AminoAcid(AminoAcidIdx type): type{type} {};
        explicit operator AminoAcidIdx const&() const;

        constexpr AminoAcid(int8_t x): AminoAcid((AminoAcidIdx)x) {};
        operator int8_t() const;

        static bool isProper(char code);
        explicit AminoAcid(char code);
        static std::string codes();
        explicit operator char const&() const;

        static bool isProper(std::string const& name);
        explicit AminoAcid(std::string const& name);
        static std::vector<std::string> names();
        explicit operator std::string() const;

        bool operator==(AminoAcid const& other) const;
        bool operator!=(AminoAcid const& other) const;
        bool operator<(AminoAcid const& other) const;
        bool operator<=(AminoAcid const& other) const;
        bool operator>(AminoAcid const& other) const;
        bool operator>=(AminoAcid const& other) const;

        AminoAcidInfo const& info() const;

        static constexpr const int N = 20;

    private:
        AminoAcidIdx type;
        friend struct std::hash<AminoAcid>;
    };
}

namespace std {
    template<>
    struct hash<mdk::AminoAcid> {
        size_t operator()(mdk::AminoAcid const &aminoAcid) const {
            return std::hash<mdk::AminoAcidIdx>()(aminoAcid.type);
        }
    };
}
