#pragma once
#include <Eigen/Geometry>
#include <data/Primitives.hpp>
#include <mdk/utils/Units.hpp>

namespace mdk {
    class SolidWall {
    public:
        double depth = 1.0 * eps;
        double cutoff = 5.0 * angstrom;
        Eigen::Hyperplane<double, 3> wall;

        void asForce(VRef v, double& V, Vector& F) const;
    };

    inline void SolidWall::asForce(VRef v, double &V, Vector &F) const {
        auto norm = wall.absDistance(v);
        if (norm < cutoff) {
            auto off = v - wall.projection(v);
            norm /= cutoff;
            V += -depth / (9 * pow(norm, 9.0));
            F += depth * (off/norm) / pow(norm, 10.0);
        }
    }
}