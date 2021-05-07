#pragma once
#include <mdk/cpu/data/State.hpp>
#include <mdk/cpu/verlet/List.hpp>
#include <mdk/cpu/verlet/Pairs.hpp>
#include <unordered_map>
#include <memory>

namespace mdk::vl {
    template<typename X0, typename... Xs>
    class Factory {
    private:
        vl::List<Xs...> cur, prev;
        using IType = typename vl::List<Xs...>::Item;
        Pairs pairs;

        template<typename X>
        void tryMove(IType &icur, IType &iprev) {
            if (iprev.template holds<X>()) {
                icur.template morph<X>(std::move(iprev.template data<X>()));
            }
        }

    public:
        void update(State const& state) {
            if (pairs.update(state)) {
                std::swap(cur, prev);
                cur.clear();

                auto iprev = prev.begin();
                for (auto const& ix: pairs.pairs) {
                    while (iprev != prev.end() && *iprev < ix)
                        ++iprev;

                    auto icur = cur.template add<X0>(ix);
                    if (*iprev == ix) (..., tryMove<Xs>(icur, iprev));
                }
            }
        }
    };
}