#include "verlet/List.hpp"
#include "simul/Simulation.hpp"
#include "forces/NonlocalForce.hpp"
#include <Eigen/Geometry>
using namespace mdk;
using namespace mdk::vl;

#include <iostream>
using namespace std;

extern Eigen::AlignedBox3d bboxTP;
extern std::vector<int> firstTP, lastTP;
extern Pairs pairsTP;

#pragma omp threadprivate(bboxTP, firstTP, lastTP, pairsTP)

Eigen::AlignedBox3d bboxTP;
std::vector<int> firstTP, lastTP;
Pairs pairsTP;

static Eigen::Vector3i floor(Eigen::Vector3d const& v) {
    return Eigen::Vector3i {
        (int)std::floor(v.x()),
        (int)std::floor(v.y()),
        (int)std::floor(v.z())
    };
}

int List::indexOf(Eigen::Vector3i const& loc) {
    return loc.x() + grid.x() * (loc.y() + grid.y() * loc.z());
}

void List::perPair(int c1, int c2) {
    for (int pt1 = first[c1]; pt1 >= 0; pt1 = next[pt1]) {
        auto r1 = state->r[pt1];
        for (int pt2 = first[c2]; pt2 >= 0; pt2 = next[pt2]) {
            if (pt1 == pt2) continue;

            auto r2 = state->r[pt2];
            auto r12_norm2 = state->top(r2 - r1).squaredNorm();

            bool cond =
                r12_norm2 <= effCutoffSq &&
                (c1 != c2 || pt1 < pt2) &&
                chains->sepByAtLeastN(pt1, pt2, minBondSep);

            if (cond) {
                pairsTP.emplace_back(min(pt1, pt2), max(pt1, pt2));
            }
        }
    }
}

void List::perCell(int c1) {
    Eigen::Vector3i loc1 {
        c1 % grid.x(),
        (c1 / grid.x()) % grid.y(),
        (c1 / grid.x()) / grid.y()
    };

    Eigen::Vector3i d;
    for (d.x() = -1; d.x() <= 1; ++d.x()) {
        for (d.y() = -1; d.y() <= 1; ++d.y()) {
            for (d.z() = -1; d.z() <= 1; ++d.z()) {
                Eigen::Vector3i loc2 = loc1 + d;
                for (int dim = 0; dim < 3; ++dim) {
                    if (loc2[dim] >= grid[dim])
                        loc2[dim] = 0;

                    if (loc2[dim] < 0)
                        loc2[dim] = grid[dim] - 1;
                }

                auto c2 = indexOf(loc2);
                if (c1 <= c2) perPair(c1, c2);
            }
        }
    }
}

void List::updateGrid() {
    Eigen::Vector3d cell;
    int gridSize;
    Eigen::AlignedBox3d bbox;

    effCutoff = cutoff + pad;
    effCutoffSq = pow(effCutoff, 2.0);

    pairs.clear();

#pragma omp parallel
    {
        bboxTP = {};

#pragma omp for schedule(dynamic, 10) nowait
        for (int i = 0; i < state->n; ++i) {
            bboxTP.extend(state->top(state->r[i]));
        }

#pragma omp critical
        {
            bbox.extend(bboxTP);
        }
#pragma omp barrier

#pragma omp single
        {
            grid = floor(bbox.sizes() / effCutoff);
            cell = Eigen::Vector3d {
                bbox.sizes().x() / grid.x(),
                bbox.sizes().y() / grid.y(),
                bbox.sizes().z() / grid.z()
            };

            gridSize = grid.x() * grid.y() * grid.z();

            first.resize(gridSize);
            std::fill(first.begin(), first.end(), -1);

            last.resize(gridSize);
            std::fill(last.begin(), last.end(), -1);

            next.resize(state->n);
            std::fill(next.begin(), next.end(), -1);
        }
#pragma omp barrier

        firstTP.resize(gridSize);
        std::fill(firstTP.begin(), firstTP.end(), -1);

        lastTP.resize(gridSize);
        std::fill(lastTP.begin(), lastTP.end(), -1);

#pragma omp for schedule(dynamic, 10) nowait
        for (int i = 0; i < state->n; ++i) {
            auto v = state->top(state->r[i]);

            Eigen::Vector3i loc = {
                (int)std::floor((v.x() - bbox.min().x()) / cell.x()),
                (int)std::floor((v.y() - bbox.min().y()) / cell.y()),
                (int)std::floor((v.z() - bbox.min().z()) / cell.z()),
            };
            for (int dim = 0; dim < 3; ++dim) {
                if (loc[dim] >= grid[dim])
                    loc[dim] = grid[dim]-1;
            }

            int c = indexOf(loc);
            if (firstTP[c] < 0) {
                firstTP[c] = lastTP[c] = i;
            } else {
                next[lastTP[c]] = i;
                lastTP[c] = i;
            }
        }

#pragma omp critical
        {
            for (int c = 0; c < gridSize; ++c) {
                if (firstTP[c] < 0)
                    continue;

                if (first[c] < 0) {
                    first[c] = firstTP[c];
                } else {
                    next[last[c]] = firstTP[c];
                }
                last[c] = lastTP[c];
            }
        }
#pragma omp barrier

        pairsTP.clear();

#pragma omp for schedule(dynamic, 10) nowait
        for (int c1 = 0; c1 < gridSize; ++c1) {
            perCell(c1);
        }

#pragma omp critical
        {
            pairs.insert(pairs.end(), pairsTP.begin(), pairsTP.end());
        }
    }

    sort(pairs.begin(), pairs.end());
    for (auto& force: forces) {
        force->vlUpdateHook();
    }
}

void List::update() {
    effCutoff = cutoff + pad;
    effCutoffSq = pow(effCutoff, 2.0);

    pairs.clear();

    #pragma omp parallel
    {
        pairsTP.clear();

        #pragma omp for schedule(dynamic, 10) nowait
        for (int pt1 = 0; pt1 < r0.size(); ++pt1) {
            auto r1 = state->r[pt1];
            for (int pt2 = pt1+1; pt2 < r0.size(); ++pt2) {
                auto r2 = state->r[pt2];
                auto r12_norm2 = state->top(r2 - r1).squaredNorm();

                bool cond = r12_norm2 <= effCutoffSq &&
                    chains->sepByAtLeastN(pt1, pt2, minBondSep);

                if (cond) pairsTP.emplace_back(pt1, pt2);
            }
        }

        #pragma omp critical
        {
            pairs.insert(pairs.end(), pairsTP.begin(), pairsTP.end());
        }
    }

    sort (pairs.begin(), pairs.end());
    for (auto& force: forces) {
        force->vlUpdateHook();
    }
}

bool List::needToReset() const {
    if (initial) return true;
    if (t0 == state->t) return false;

    auto maxMoveSq = 0.0;
    for (int i = 0; i < state->n; ++i) {
        auto moveSq = (state->r[i] - r0[i]).squaredNorm();
        maxMoveSq = std::max(maxMoveSq, moveSq);
    }

    auto maxMove = sqrt(maxMoveSq);
    auto pbcShift = (top0.cell - state->top.cell).lpNorm<1>();
    return maxMove + 2.0 * pbcShift >= pad / 2.0;
}

void List::bind(Simulation &simulation) {
    state = &simulation.var<State>();
    chains = &simulation.data<Chains>();
    initial = true;
}

void List::check() {
    if (needToReset()) {
        t0 = state->t;
        r0 = state->r;
        top0 = state->top;
        update();
        // For benchmarking, it's important
        // to keep track of VL size to ensure we do the same amount of stuff
        cout << "PAIRS SIZE = " << pairs.size() << endl;
    }
    initial = false;
}

void List::registerNF(NonlocalForce& force, Spec const& spec) {
    cutoff = std::max(cutoff, sqrt(spec.cutoffSq));
    if (minBondSep < 1) minBondSep = spec.minBondSep;
    else minBondSep = std::min(minBondSep, spec.minBondSep);

    forces.push_back(&force);
}
