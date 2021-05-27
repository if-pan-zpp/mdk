#pragma once
#include "Stat.hpp"
#include "../simul/SimulVar.hpp"
#include <vector>
#include "../data/Types.hpp"
#include "../files/param/Parameters.hpp"

namespace mdk {
    class Stats: public SimulVar {
    private:
        Types const *types = nullptr;
        std::vector<param::Polarization> polarization;

    public:
        std::vector<Stat> stats;
        bool used = false;

        void bind(Simulation& simulation) override;

        enum class Type: int8_t {
            BB, BS, SB, SS
        };

        inline void creationDiffs(int i1, int i2, Type type, Stat diffs[2]) const {
            diffs[0] = diffs[1] = Stat();

            switch (type) {
            case Type::BB:
                --diffs[0].backbone;
                --diffs[1].backbone;
                break;
            case Type::BS:
                --diffs[0].backbone;
                --diffs[1].sidechain;
                break;
            case Type::SB:
                --diffs[0].sidechain;
                --diffs[1].backbone;
                break;
            case Type::SS:
                --diffs[0].sidechain;
                --diffs[1].sidechain;
            }

            if (type == Type::SS) {
                int idx[2] = {i1, i2};
                for (int k = 0; k < 2; ++k) {
                    switch (polarization[(int8_t)(*types)[idx[1-k]]]) {
                    case param::Polarization::POLAR:
                    case param::Polarization::POLAR_NEG:
                    case param::Polarization::POLAR_POS:
                        --diffs[k].polarSS;
                        break;

                    case param::Polarization::HYDROPHOBIC:
                        --diffs[k].hydrophobicSS;
                        break;

                    default:
                        break;
                    }
                }
            }
        }
    };
}