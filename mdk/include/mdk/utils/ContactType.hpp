#pragma once
#include <vector>
#include <cstdint>
#include <string>

namespace mdk {
    enum class ContactTypeIdx: int8_t {
        NAT, NAT_BB, NAT_BS, NAT_SB, NAT_SS, SSBOND
    };

    class ContactType {
    public:
        ContactType() = default;

        constexpr explicit ContactType(ContactTypeIdx code): code {code} {};
        explicit operator ContactTypeIdx const&() const;

        constexpr ContactType(int8_t x): ContactType((ContactTypeIdx)x) {};
        operator int8_t() const;

        explicit ContactType(std::string const& name);
        explicit operator std::string() const;

    private:
        ContactTypeIdx code;
        friend struct std::hash<ContactType>;
    };
}

namespace std {
    template<>
    struct hash<mdk::ContactType> {
        size_t operator()(mdk::ContactType const &resType) const {
            return std::hash<mdk::ContactTypeIdx>()(resType.code);
        }
    };
}