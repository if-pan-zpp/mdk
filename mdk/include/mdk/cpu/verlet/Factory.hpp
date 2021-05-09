#pragma once
#include <mdk/cpu/data/State.hpp>
#include <mdk/cpu/verlet/VL.hpp>
#include <mdk/cpu/verlet/Pairs.hpp>
#include <unordered_map>
#include <memory>

namespace mdk::vl {
    template<typename VL, typename... Xs>
    class Factory {
    private:
        using X0 = typename std::tuple_element<0, std::tuple<Xs...>>::type;

        VL prev;
        using IType = typename VL::Item;
        Pairs pairs;

        template<typename X>
        void tryMove(IType &icur, IType &iprev) {
            if (iprev.template holds<X>()) {
                icur.template morph<X>(std::move(iprev.template data<X>()));
            }
        }

    public:
        VL cur;

        template<typename... Args>
        explicit Factory(Args&&... args): prev{args...}, cur{args...} {};

        void update(State const& state) {
            if (pairs.needToReset(state)) {
                pairs.cutoff = cur.cutoff();
                pairs.update(state);

                std::swap(cur, prev);
                cur.clear();

                auto iprev = prev.begin();
                for (auto const&[i1, i2]: pairs.pairs) {
                    vl::Base base;
                    base.i1 = i1;
                    base.i2 = i2;

                    while (iprev != prev.end() && iprev.base() < base)
                        ++iprev;

                    auto icur = cur.template add<X0>(base);
                    if (iprev != prev.end() && iprev.base() == base)
                        (..., tryMove<Xs>(icur, iprev));
                }
            }
        }
    };
}