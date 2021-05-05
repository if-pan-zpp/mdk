#pragma once
#include <mdk/cpu/data/State.hpp>
#include <mdk/cpu/verlet/List.hpp>
#include <unordered_map>
#include <memory>

namespace mdk::vl {
    template<typename Data>
    class Factory {
    private:
        std::vector<std::unique_ptr<vl::List<Data>>> vls;
        std::vector<std::vector<vl::Pair<Data>>> prevVLs;

        State const* state0;
        Vectors r0;
        Topology top0;

        double pad;
        Eigen::Vector3i grid;
        std::vector<int> first, next, last;

        void reconstruct() {
            prepareCells();
            createLists();
            saveRefValues();
        }

        int index(Eigen::Vector3i const& loc) const {
            return loc.x() + grid.x() * (loc.y() + grid.y() * loc.z());
        }

        static Eigen::Vector3i floor(Eigen::Vector3d const& v) {
            return { std::floor(v.x()), std::floor(v.y()), std::floor(v.z()) };
        }

        void prepareCells() {
            Eigen::AlignedBox3d bbox;
            for (auto const& pt: state0->r) {
                bbox.extend(state0->top(pt));
            }

            auto maxCutoff2 = 0.0;
            for (auto const& vl: vls) {
                maxCutoff2 = std::max(maxCutoff2, vl.cutoff2());
            }

            auto effCutoff = sqrt(maxCutoff2) + pad;
            grid = floor(bbox.sizes() / effCutoff);

            first.resize(grid.x() * grid.y() * grid.z(), -1);
            last.resize(grid.x() * grid.y() * grid.z(), -1);
            next.resize(state0->n, -1);

            for (int i = 0; i < state0->n; ++i) {
                auto r = state0->top(state0->r[i]);
                auto loc = floor((r - bbox.min()) / effCutoff);
                int cellIdx = index(loc);

                if (first[cellIdx] < 0) {
                    first[cellIdx] = last[cellIdx] = i;
                } else {
                    next[last[cellIdx]] = i;
                    last[cellIdx] = i;
                }
            }
        }

        void createLists() {
            for (auto& vl: vls) {
                vl->pairs.clear();
            }

            traverseOuter();

            for (auto& vl: vls) {
                std::sort(vl->pairs.begin(), vl->pairs.end());
                vl->finish();
            }
        }

        void traverseOuter() {
            Eigen::Vector3i loc1;
            for (loc1.x() = 0; loc1.x() < grid.x(); ++loc1.x()) {
                for (loc1.y() = 0; loc1.y() < grid.y(); ++loc1.y()) {
                    for (loc1.z() = 0; loc1.z() < grid.z(); ++loc1.z()) {
                        traverseInner(index(loc1), loc1);
                    }
                }
            }
        }

        void traverseInner(int index1, Eigen::Vector3i const& loc1) {
            Eigen::Vector3i diff;
            for (diff.x() = -1; diff.x() <= 1; ++diff.x()) {
                for (diff.y() = -1; diff.y() <= 1; ++diff.y()) {
                    for (diff.z() = -1; diff.z() <= 1; ++diff.z()) {
                        Eigen::Vector3i loc2 = loc1 + diff;
                        for (int d = 0; d < 3; ++d) {
                            if (loc2[d] < 0) loc2[d] += grid[d];
                        }

                        auto index2 = index(loc2);
                        if (index1 <= index2) {
                            forCellPair(index1, index2);
                        }
                    }
                }
            }
        }

        void forCellPair(int index1, int index2) {
            for (int pt1 = first[index1]; pt1 >= 0; pt1 = next[pt1]) {
                Vector r1 = state0->r[pt1];
                for (int pt2 = first[index2]; pt2 >= 0; pt2 = next[pt2]) {
                    if (pt1 >= pt2) continue;

                    Vector r2 = state0->r[pt2];
                    double r12_norm2 = state0->top(r2 - r1).squaredNorm();
                    for (auto& vl: vls) {
                        if (r12_norm2 <= vl->cutoff2) {
                            vl->pairs.emplace_back(pt1, pt2, Data());
                        }
                    }
                }
            }
        }

        void saveRefValues() {
            r0 = state0->r;
            top0 = state0->top;
        }

        bool needToUpdate() {
            double maxMove2 = 0.0;
            for (int i = 0; i < state0->n; ++i) {
                double move2 = (state0->r[i] - r0[i]).squaredNorm();
                maxMove2 = std::max(maxMove2, move2);
            }
            double maxMove = sqrt(maxMove2);

            double pbcMove = (top0.cell - state0->top.cell).norm();
            double maxAllowed = 0.5 * pad - 2.0 * pbcMove;

            return maxMove >= maxAllowed;
        }

        void restoreData(std::vector<vl::Pair<Data>>& curVL,
                std::vector<vl::Pair<Data>>& prevVL) {
            int iprev = 0;
            for (int icur = 0; icur < curVL.size(); ++icur) {
                if (iprev >= prevVL.size())
                    break;

                auto& cur = curVL[icur];

                while (iprev < prevVL.size() && prevVL[iprev] < cur) {
                    ++iprev;
                }

                if (iprev < prevVL.size()) {
                    auto& prev = prevVL[iprev];
                    if (cur.i1 == prev.i1 && cur.i2 == prev.i2)
                        std::swap(cur.data, prev.data);
                }
            }
        }

    public:
        void update(State const& state) {
            if (this->state0 != &state) {
                this->state0 = &state;
                reconstruct();
            }
            else if (needToUpdate()) {
                for (int i = 0; i < vls.size(); ++i) {
                    std::swap(vls[i]->pairs, prevVLs);
                }

                reconstruct();

                for (int i = 0; i < vls.size(); ++i) {
                    restoreData(vls[i]->pairs, prevVLs[i]);
                }
            }
        }

        template<typename ListImpl>
        vl::List<Data>& attachList(ListImpl const& list) {
            vls.emplace_back(std::make_unique<vl::List<Data>>(list));
            prevVLs.emplace_back();

            return vls.back();
        }
    };
}