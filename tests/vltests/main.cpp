#include <iostream>
#include <eigen3/Eigen/Geometry>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace Eigen;
using namespace std::chrono;

extern Eigen::AlignedBox3d bboxTP;
extern vector<int> firstTP, lastTP;
extern vector<pair<int, int>> pairsTP;

#pragma omp threadprivate(bboxTP, firstTP, lastTP, pairsTP)

Eigen::AlignedBox3d bboxTP;
std::vector<int> firstTP, lastTP;
vector<pair<int, int>> pairsTP;

class CellularVL {
public:
    Vector3i grid;
    std::vector<int> first, last, next;
    double r, r_sq;
    Matrix3Xd const &m;

    CellularVL(double r, Matrix3Xd const &m) : r(r), r_sq(r * r), m(m) {};

    Vector3i floor(Vector3d const &v) {
        return Vector3i{
            (int) std::floor(v.x()),
            (int) std::floor(v.y()),
            (int) std::floor(v.z())};
    }

    int indexOf(Vector3i const &loc) {
        return loc.x() + grid.x() * (loc.y() + grid.y() * loc.z());
    }

    void perPair(int c1, int c2) {
        for (int pt1 = first[c1]; pt1 >= 0; pt1 = next[pt1]) {
            auto r1 = m.col(pt1);
            for (int pt2 = first[c2]; pt2 >= 0; pt2 = next[pt2]) {
                if (pt1 == pt2) continue;

                auto r2 = m.col(pt2);
                auto r12_norm2 = (r2 - r1).squaredNorm();

                if (r12_norm2 <= r_sq && (c1 != c2 || pt1 < pt2)) {
                    pairsTP.emplace_back(min(pt1, pt2), max(pt1, pt2));
                }
            }
        }
    }

    void perCell(int c1) {
        Vector3i loc1 {
            c1 % grid.x(),
            (c1 / grid.x()) % grid.y(),
            (c1 / grid.x()) / grid.y()
        };

        Vector3i d;
        for (d.x() = -1; d.x() <= 1; ++d.x()) {
            for (d.y() = -1; d.y() <= 1; ++d.y()) {
                for (d.z() = -1; d.z() <= 1; ++d.z()) {
                    Vector3i loc2 = loc1 + d;
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

    vector<pair<int, int>> pairs;

    void compute() {
        Vector3d cell;
        int gridSize;
        AlignedBox3d bbox;

#pragma omp parallel
        {
            bboxTP = {};

#pragma omp for schedule(dynamic, 10) nowait
            for (int i = 0; i < m.cols(); ++i) {
                bboxTP.extend(m.col(i));
            }

#pragma omp critical
            {
                bbox.extend(bboxTP);
            }
#pragma omp barrier

#pragma omp single
            {
                grid = floor(bbox.sizes() / r);
                cell = Vector3d{
                    bbox.sizes().x() / grid.x(),
                    bbox.sizes().y() / grid.y(),
                    bbox.sizes().z() / grid.z()
                };

                gridSize = grid.x() * grid.y() * grid.z();

                first.resize(gridSize);
                std::fill(first.begin(), first.end(), -1);

                last.resize(gridSize);
                std::fill(last.begin(), last.end(), -1);

                next.resize(m.cols());
                std::fill(next.begin(), next.end(), -1);
            }
#pragma omp barrier

            firstTP.resize(gridSize);
            std::fill(firstTP.begin(), firstTP.end(), -1);

            lastTP.resize(gridSize);
            std::fill(lastTP.begin(), lastTP.end(), -1);

#pragma omp for schedule(dynamic, 10) nowait
            for (int i = 0; i < m.cols(); ++i) {
                auto v = m.col(i);

                Vector3i loc = {
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

            pairs.clear();

#pragma omp critical
            {
                pairs.insert(pairs.end(), pairsTP.begin(), pairsTP.end());
            }
        }

        sort(pairs.begin(), pairs.end());
    }
};

class SquareVL {
public:
    double r, r_sq;
    Matrix3Xd const &m;

    SquareVL(double r, Matrix3Xd const &m) : r(r), r_sq(r * r), m(m) {};

    vector<pair<int, int>> pairs;

    void compute() {
        pairs.clear();

#pragma omp parallel
        {
            pairsTP.clear();

#pragma omp for schedule(dynamic, 10) nowait
            for (int pt1 = 0; pt1 < m.cols(); ++pt1) {
                auto r1 = m.col(pt1);
                for (int pt2 = pt1 + 1; pt2 < m.cols(); ++pt2) {
                    auto r2 = m.col(pt2);
                    double r12_norm2 = (r2 - r1).squaredNorm();

                    if (r12_norm2 <= r_sq)
                        pairsTP.emplace_back(pt1, pt2);
                }
            }

#pragma omp critical
            {
                pairs.insert(pairs.end(), pairsTP.begin(), pairsTP.end());
            }
        }

        sort(pairs.begin(), pairs.end());
    }
};

Matrix3Xd gen(int n, double r, double avg_neigh) {
    double density = avg_neigh / (4.0 / 3.0 * M_PI * r * r * r);
    double a = pow(n / density, 1.0 / 3.0);
    return Matrix3Xd::Random(3, n) * (a / 2.0);
}

int main() {
    omp_set_num_threads(8);
    double r = 30.0, avg_neigh = 10.0;
    int n = 100'000;

    auto v = gen(n, r, avg_neigh);

    auto vl1 = SquareVL(r, v);
    auto then1 = high_resolution_clock::now();
    vl1.compute();
    auto now1 = high_resolution_clock::now();
    auto dur1 = duration_cast<microseconds>(now1 - then1).count();
    cout << "Square: #Pairs = " << vl1.pairs.size() << "; t = " << dur1 << '\n';

    auto vl2 = CellularVL(r, v);
    auto then2 = high_resolution_clock::now();
    vl2.compute();
    auto now2 = high_resolution_clock::now();
    auto dur2 = duration_cast<microseconds>(now2 - then2).count();
    cout << "Cell: #Pairs = " << vl2.pairs.size() << "; t = " << dur2 << '\n';

    return 0;
}