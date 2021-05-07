#pragma once
#include <vector>
#include <random>
#include <Eigen/Core>

namespace mdk {
    class Random {
    public:
        virtual double uniform() = 0;
        double uniform(double a, double b);

        virtual double normal();
        double normal(double mu, double sigma);

        virtual Eigen::Vector3d sphere();
    };

    class ModernRandom: public Random {
    private:
        std::mt19937 eng;
        std::uniform_real_distribution<double> unifDist;
        std::normal_distribution<double> normalDist;

    public:
        explicit ModernRandom(int seed = 0):
            eng(seed), unifDist(0, 1), normalDist(0, 1) {};

        double uniform() override;
        double normal() override;
    };

    class FortranRandom: public Random {
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
            am = 1.0/im1;

        int iy = 0, idum = -448, idum2 = 123456789;
        int iv[ntab];

    public:
        explicit FortranRandom(int seed = 0);
        double uniform() override;
    };
}
