#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace mdk {
    enum class AAType: int8_t {
        ALA, ARG, ASN, ASP, CYS, GLU, GLN, GLY, HIS, ILE,
        LEU, LYS, MET, PHE, PRO, SER, THR, TRP, TYR, VAL
    };

    class AminoAcid {
    public:
        AminoAcid() = default;
        static std::vector<AminoAcid> aminoAcids();

        static std::vector<AAType> types();
        constexpr explicit AminoAcid(AAType type): type{type} {};
        explicit operator AAType const&() const;

        constexpr explicit AminoAcid(int8_t x): AminoAcid((AAType)x) {};
        explicit operator int8_t() const;

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

        static constexpr const int N = 20;

    private:
        AAType type;
        friend struct std::hash<AminoAcid>;
    };
}

namespace std {
    template<>
    struct hash<mdk::AminoAcid> {
        size_t operator()(mdk::AminoAcid const &aminoAcid) const {
            return std::hash<mdk::AAType>()(aminoAcid.type);
        }
    };
}
