#pragma once
#include <cstdint>
#include <vector>
#include <utility>
#include <tuple>

namespace mdk {
    template<typename...>
    struct Index:
        std::integral_constant<int8_t, -1> {};

    template<typename T, typename... R>
    struct Index<T, T, R...>:
        std::integral_constant<int8_t, 0> {};

    template<typename T, typename F, typename... R>
    struct Index<T, F, R...>:
        std::conditional<
            Index<T, R...>::value != -1,
            std::integral_constant<int8_t, 1 + Index<T, R...>::value>,
        std::integral_constant<int8_t, -1>>::type {};

    template<typename X>
    using Extras = std::vector<X>;

    template<typename Base>
    class Boxed {
    public:
        Base x;
        int8_t type;
        int dataIdx;

        Boxed() = default;
        Boxed(Base x, int8_t type, int dataIdx):
            x(std::move(x)), type(type), dataIdx(dataIdx) {};

        bool operator<(Boxed const& other) {
            return x < other.x;
        }
    };

    template<typename Base, typename... Xs>
    class Metavector {
    public:
        std::vector<Boxed<Base>> basis;
        std::tuple<Extras<Xs>...> extras;

        class Item {
        private:
            friend class Metavector;
            Metavector* owner;

            using TrueIter = typename decltype(basis)::iterator;
            TrueIter iter;

            explicit Item(Metavector& owner, TrueIter const& iter):
                owner {&owner}, iter {iter} {};

        public:
            Item& operator++() {
                ++iter;
                return *this;
            }

            Item operator++(int) {
                auto sav = *this;
                ++*this;
                return sav;
            }

            bool operator==(Item const& other) const {
                return iter == other.iter;
            }

            bool operator!=(Item const& other) const {
                return iter != other.iter;
            }

            Item& operator*() {
                return *this;
            }

            Base* operator->() {
                return &iter->x;
            }

        public:
            template<typename X>
            bool holds() {
                return Index<X, Xs...>::value == iter->type;
            }

            Base& base() {
                return iter->x;
            }

            template<typename X>
            X& data() {
                auto& dataX = std::get<Index<X, Xs...>::value>(owner->extras);
                return dataX[iter->dataIdx];
            }

            template<typename Y, typename... Args>
            Y& morph(Args&&... args) {
                constexpr auto indY = Index<Y, Xs...>::value;
                auto& dataY = std::get<indY>(owner->extras);

                iter->type = indY;
                auto iterY = dataY.emplace(dataY.end(), std::forward<Args>(args)...);
                iter->dataIdx = iterY - dataY.begin();
                return *iterY;
            }
        };

        Item begin() {
            return Item(*this, basis.begin());
        }

        Item end() {
            return Item(*this, basis.end());
        }

        template<typename X>
        Item add(Base const& b = Base(), X const& x = X()) {
            constexpr int8_t indX = Index<X, Xs...>::value;
            auto& dataX = std::get<indX>(extras);
            auto xIter = dataX.emplace(dataX.end(), x);

            auto baseIter = basis.emplace(basis.end(),
                std::move(b), indX, xIter - dataX.begin());
            return Item(*this, baseIter);
        }

        Item operator[](int idx) {
            return Item(*this, basis.begin() + idx);
        }

        void clear() {
            basis.clear();
            (..., std::get<Index<Xs, Xs...>::value>(extras).clear());
        }
    };
}