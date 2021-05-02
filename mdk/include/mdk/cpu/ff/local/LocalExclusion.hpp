#pragma once
#include <mdk/utils/Units.hpp>

namespace mdk {
    class LocalExclusion {
    private:
        double cutoff = 5.0 * angstrom;
        double cutoff2 = pow(cutoff, 2.0);

    public:
        bool kernel(double l2, double& l, double &V, double &dV_dl);
    };

    inline bool LocalExclusion::kernel(double l2, double& l, double &V, double &dV_dl) {
        if (l2 >= cutoff2) {
            return false;
        }
        else {
            l = sqrt(l);
            auto s = cutoff / l;
            auto s6 = pow(s, 6.0);
            V += eps * (s6*s6 - 2.0 * s6 + 1.0);
            dV_dl += 12.0 * (s6*s6 - s6) / l;
            return true;
        }
    }
}