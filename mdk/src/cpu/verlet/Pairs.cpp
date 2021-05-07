#include "cpu/verlet/Pairs.hpp"
using namespace mdk;
using namespace mdk::vl;

int Pairs::index(const Eigen::Vector3i &loc) const {
    return loc.x() + grid.x() * (loc.y() + grid.y() * loc.z());
}

static Eigen::Vector3i floor(const Vector &v) {
    return { std::floor(v.x()), std::floor(v.y()), std::floor(v.z()) };
}

void Pairs::prepCells() {
    Eigen::AlignedBox3d bbox;
    for (auto const& pt: state0->r) {
        bbox.extend(state0->top(pt));
    }

    auto effCutoff = cutoff + 2.0 * pad;
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

void Pairs::cellLoop() {
    Eigen::Vector3i loc1;
    for (loc1.x() = 0; loc1.x() < grid.x(); ++loc1.x()) {
        for (loc1.y() = 0; loc1.y() < grid.y(); ++loc1.y()) {
            for (loc1.z() = 0; loc1.z() < grid.z(); ++loc1.z()) {
                neighLoop(index(loc1), loc1);
            }
        }
    }
}

void Pairs::neighLoop(int idx1, const Eigen::Vector3i &loc1) {
    Eigen::Vector3i diff;
    for (diff.x() = -1; diff.x() <= 1; ++diff.x()) {
        for (diff.y() = -1; diff.y() <= 1; ++diff.y()) {
            for (diff.z() = -1; diff.z() <= 1; ++diff.z()) {
                Eigen::Vector3i loc2 = loc1 + diff;
                for (int d = 0; d < 3; ++d) {
                    if (loc2[d] < 0) loc2[d] += grid[d];
                }

                auto idx2 = index(loc2);
                if (idx1 <= idx2) {
                    forCellPair(idx1, idx2);
                }
            }
        }
    }
}

void Pairs::forCellPair(int idx1, int idx2) {
    auto cutoffSq = cutoff*cutoff;
    for (int pt1 = first[idx1]; pt1 >= 0; pt1 = next[pt1]) {
        Vector r1 = state0->r[pt1];
        for (int pt2 = first[idx2]; pt2 >= 0; pt2 = next[pt2]) {
            if (pt1 >= pt2) continue;

            Vector r2 = state0->r[pt2];
            double r12_norm2 = state0->top(r2 - r1).squaredNorm();
            if (r12_norm2 <= cutoffSq)
                pairs.emplace_back(pt1, pt2);
        }
    }
}

void Pairs::genPairList() {
    prepCells();
    cellLoop();
    std::sort(pairs.begin(), pairs.end());
}

bool Pairs::needToReset() const {
    double maxMoveSq = 0.0;
    for (int i = 0; i < state0->n; ++i) {
        double moveSq = (state0->r[i] - r0[i]).squaredNorm();
        maxMoveSq = std::max(maxMoveSq, moveSq);
    }

    double maxMove = sqrt(maxMoveSq);
    double pbcShift = (top0.cell - state0->top.cell).lpNorm<1>();
    return maxMove + 2.0 * pbcShift >= pad;
}

bool Pairs::update(State const& state) {
    auto reset = needToReset();

    if (needToReset()) {
        this->state0 = &state;
        prepCells();
        genPairList();
        r0 = state.r;
        top0 = state.top;
    }
    return reset;
}