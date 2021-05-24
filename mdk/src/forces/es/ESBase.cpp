#include "forces/es/ESBase.hpp"
#include "simul/Simulation.hpp"
#include "runtime/Lambda.hpp"
using namespace mdk;

void ESBase::bind(Simulation &simulation) {
    NonlocalForce::bind(simulation);

    auto& model = *simulation.data<Model>();
    auto& params = *simulation.data<param::Parameters>();

    charge.resize(model.n);
    for (int i = 0; i < model.n; ++i) {
        AminoAcid acid((int8_t)model.residues[i].type);
        auto pol = params.specificity.at(acid).polarization;

        if (pol == param::Polarization::POLAR_POS) {
            charge[i] = 1;
        }
        else if (pol == param::Polarization::POLAR_NEG) {
            charge[i] = -1;
        }
        else {
            charge[i] = 0;
        }
    }

    auto update = [&]() -> {
        pairs.clear();
        for (auto const& [i1, i2]: vl) {
            auto q1_x_q2 = charge[i1] * charge[i2];
            if (q1_x_q2 != 0) {
                pairs.emplace_back((Contact) {
                    .i1 = i1, .i2 = i2, .q1_x_q2 = q1_x_q2
                });
            }
        }
    };
    auto updateTask = Lambda({}, update, {}).unique();
    vl->updateScheduler.asyncUpdates.emplace_back(std::move(updateTask));
}
