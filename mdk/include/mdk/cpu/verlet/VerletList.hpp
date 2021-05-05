#pragma once
#include <vector>
#include <memory>

namespace mdk::vl {
    template<typename Data>
    struct Pair {
        int i1, i2;
        Data data;

        bool operator<(Pair<Data> const& other) const {
            return std::make_pair(i1, i2) < std::make_pair(other.i1, other.i2);
        }
    };

    template<typename Data>
    struct Mod {
    public:
        virtual double cutoff2() const = 0;
        virtual void modify(std::vector<vl::Pair<Data>>& pairs) const = 0;
    };

    template<typename Data>
    class List {
    public:
        std::vector<vl::Pair<Data>> pairs;
        std::vector<std::unique_ptr<vl::Mod<Data>>> mods;

        template<typename ModImpl>
        void attachMod(ModImpl const& mod) {
            mods.emplace_back(std::make_unique<vl::Mod<Data>>(mod));
        }

        double cutoff2() const {
            double maxCutoff2 = 0.0;
            for (auto const& mod: mods) {
                maxCutoff2 = std::max(maxCutoff2, mod.cutoff2());
            }
            return maxCutoff2;
        }

        double filter() {
            for (auto const& mod: mods) {
                mod.modify(pairs);
            }
        }
    };
}
