#pragma once
#include <vector>
#include <random>
#include <Eigen/Core>

namespace mdk {
    class Random {
    private:
        static constexpr int
            im1 = 2147483563,
            im2 = 2147483399,
            imm1 = im1-1,
            ia1 = 40014,
            ia2 = 40692,
            iq1 = 53668,
            iq2 = 52774,
            ir1 = 12211,
            ir2 = 3791,
            ntab = 32,
            ndiv = 1+imm1/ntab;

        static constexpr double
            eps = 1.2e-7,
            rnmx = 1.-eps,
            am = (float) 1.0/im1;

        int iy = 0, idum = -448, idum2 = 123456789;
        int iv[ntab];

    public:
        Random(int seed) {
            idum = -seed;
            for (auto& x: iv) x = 0;
        }

        inline double uniform() {
            int k, j;
            if (idum <= 0) {
                idum2 = idum = std::max(-idum, 1);
                for (j = ntab + 7; j >= 0; --j) {
                    k = idum / iq1;
                    idum = ia1 * (idum - k * iq1) - k * ir1;
                    if (idum < 0) idum += im1;
                    if (j < ntab) iv[j] = idum;
                }
                iy = iv[0];
            }

            k = idum / iq1;
            idum = ia1 * (idum - k * iq1) - k * ir1;
            if (idum < 0) idum += im1;

            k = idum2 / iq2;
            idum2 = ia2 * (idum2 - k * iq2) - k * ir2;
            if (idum2 < 0) idum2 += im2;

            j = iy / ndiv;
            iy = iv[j] - idum2;
            iv[j] = idum;
            if (iy < 1) iy += imm1;

            return std::min(am * iy, rnmx);
        }

        inline double uniform(double a, double b) {
            return a + (b - a) * uniform();
        }

        inline double normal() {
            double r1 = uniform();
            double r2 = uniform();
            return sqrt(-2.0  * log(r1)) * cos(2.0 * M_PI * r2);
        }

        inline double normal(double mu, double sigma) {
            return mu + sigma * normal();
        }

        inline Eigen::Vector3d sphere() {
            Eigen::Vector3d r { normal(), normal(), normal() };
            return r.normalized();
        }
    };
}
