#include "forces/es/ESBase.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

void ESBase::bind(Simulation &simulation) {
    NonlocalForce::bind(simulation);

    auto& model = simulation.data<Model>();
    auto& params = simulation.data<param::Parameters>();

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

    installIntoVL();
}

void ESBase::vlUpdateHook() {
    pairs.clear();
    for (auto const& [i1, i2]: vl->pairs) {
        auto q1_x_q2 = charge[i1] * charge[i2];
        if (q1_x_q2 != 0) {
            pairs.emplace_back((Contact) {
                .i1 = i1, .i2 = i2, .q1_x_q2 = (double)q1_x_q2
            });
        }
    }
}

void ESBase::serialize(std::ostream &ostream) {
    ostream << pairs.size() << '\n';
    for (Contact const &c: pairs) {
        ostream << c.i1 << ' ' << c.i2 << ' ' << *(reinterpret_cast<const uint64_t*>(&c.q1_x_q2)) << '\n';
    }
}

void ESBase::deserialize(std::istream &istream) {
    size_t size;
    istream >> size;
    pairs.resize(size);
    for (Contact &c: pairs) {
        istream >> c.i1 >> c.i2 >> *(reinterpret_cast<uint64_t*>(&c.q1_x_q2));
    }
}
