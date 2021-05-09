#pragma once
#include <vector>
#include <cstdint>
#include <string>

namespace mdk {
    enum class ContCode: int8_t {
        NAT, NAT_BB, NAT_BS, NAT_SB, NAT_SS, SSBOND
    };

    class ContactType {
    public:
        ContactType() = default;

        constexpr explicit ContactType(ContCode code): code {code} {};
        explicit operator ContCode const&() const;

        constexpr ContactType(int8_t x): ContactType((ContCode)x) {};
        operator int8_t() const;

        explicit ContactType(std::string const& name);
        explicit operator std::string() const;

    private:
        ContCode code;
        friend struct std::hash<ContactType>;
    };
}

namespace std {
    template<>
    struct hash<mdk::ContactType> {
        size_t operator()(mdk::ContactType const &resType) const {
            return std::hash<mdk::ContCode>()(resType.code);
        }
    };
}