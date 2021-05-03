#pragma once
#include <mdk/cpu/data/State.hpp>
#include <mdk/cpu/verlet/VerletList.hpp>
#include <mdk/cpu/verlet/VLSpec.hpp>
#include <unordered_map>
#include <memory>

namespace mdk {
    class VLFactory {
    private:
        State const* state;
        using VLPair = std::pair<std::unique_ptr<VLSpec>, VerletList>;
        std::vector<VLPair> lists;

        double pad;
        Vectors r0;
        Topology top0;

        std::vector<int> first, next, cur;

        int nx, ny, nz;
        void prepareCells();
        void prepareLists();
        void forCellPair(int cell1, int cell2);
        void traverseCellPairs();
        void completeLists();

    public:
        explicit VLFactory(State const& state);

        template<typename Spec>
        VerletList& createVL(Spec const& spec) {
            lists.emplace_back(std::make_unique<Spec>(spec), VerletList());
        }

        void update();
    };
}