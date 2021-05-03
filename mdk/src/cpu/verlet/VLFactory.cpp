#include "cpu/verlet/VLFactory.hpp"
#include <Eigen/Geometry>
#include <algorithm>
using namespace mdk;
using namespace std;

#define index(ix, iy, iz) (ix + nx * iy + nx * ny * iz)

VLFactory::VLFactory(const State &state) {
    this->state = &state;
    cur = next = std::vector<int>(state.n, -1);
}

void VLFactory::prepareCells() {
    Eigen::AlignedBox3d bbox;
    for (auto const& pt: state->r)
        bbox.extend(state->top(pt));

    static auto vlComp = [](VLPair const& vl1, VLPair const& vl2) -> bool {
        return vl1.first->cutoff < vl2.first->cutoff;
    };
    auto maxIter = max_element(lists.begin(), lists.end(), vlComp);
    auto maxCutoff = maxIter->first->cutoff;
    auto effCutoff = maxCutoff + pad;

    nx = floor(bbox.sizes().x() / effCutoff);
    ny = floor(bbox.sizes().y() / effCutoff);
    nz = floor(bbox.sizes().z() / effCutoff);

    first.clear();
    first.resize(nx * ny * nz, -1);
    std::fill(next.begin(), next.end(), -1);

    for (int i = 0; i < state->n; ++i) {
        auto fixedr = state->top(state->r[i]);
        auto scaledr = (fixedr - bbox.min()) / effCutoff;
        int ix = floor(scaledr.x()), iy = floor(scaledr.y()),
            iz = floor(scaledr.z());

        auto cellIdx = index(ix, iy, iz);
        if (first[cellIdx] < 0) {
            first[cellIdx] = cur[cellIdx] = i;
        }
        else {
            next[cur[cellIdx]] = i;
            cur[cellIdx] = i;
        }
    }
}

void VLFactory::prepareLists() {
    for (auto& [spec, list]: lists) {
        list.clear();
    }
}

void VLFactory::forCellPair(int cell1, int cell2) {
    for (int p1 = first[cell1]; p1 >= 0; p1 = next[p1]) {
        auto r1 = state->r[p1];
        for (int p2 = first[cell2]; p2 >= 0; p2 = next[p2]) {
            if (p1 >= p2) continue;

            auto r2 = state->r[p2];
            auto norm12 = state->top(r2 - r1).norm();

            for (auto& [spec, list]: lists) {
                if (norm12 <= spec->cutoff)
                    list.emplace_back(p1, p2);
            }
        }
    }
}

void VLFactory::traverseCellPairs() {
    for (int cellx = 0; cellx < nx; ++cellx) {
        for (int celly = 0; celly < ny; ++celly) {
            for (int cellz = 0; cellz < nz; ++cellz) {
                int cellIdx = index(cellx, celly, cellz);

                for (int dx = -1; dx < 2; ++dx) {
                    int neighx = cellx + dx;
                    if (neighx < 0) neighx += nx;

                    for (int dy = -1; dy < 2; ++dy) {
                        int neighy = celly + dy;
                        if (neighy < 0) neighy += ny;

                        for (int dz = -1; dz < 2; ++dz) {
                            int neighz = cellz + dz;
                            if (neighz < 0) neighz += nz;

                            int neighIdx = index(neighx, neighy, neighz);
                            if (cellIdx <= neighIdx)
                                forCellPair(cellIdx, neighIdx);
                        }
                    }
                }
            }
        }
    }
}

void VLFactory::completeLists() {
    for (auto& [spec, list]: lists) {
        sort(list.begin(), list.end());
        spec->filter(list);
    }
}

void VLFactory::reconstruct() {
    prepareCells();
    prepareLists();
    traverseCellPairs();
    completeLists();
    r0 = state->r;
    top0 = state->top;
}

void VLFactory::update() {
    double maxMove2 = 0.0;
    for (int i = 0; i < state->n; ++i) {
        auto dr = state->r[i] - r0[i];
        maxMove2 = max(maxMove2, dr.squaredNorm());
    }

    auto maxMove = sqrt(maxMove2);
    auto pbcMove = (top0.cell - state->top.cell).norm();
    auto maxAllowedMove = 0.5 * pad - 2.0 * pbcMove;
    if (maxMove2 >= maxAllowedMove) {
        reconstruct();
    }
}