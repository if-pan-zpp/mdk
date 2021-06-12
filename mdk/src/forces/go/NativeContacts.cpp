#include "forces/go/NativeContacts.hpp"
#include "kernels/LennardJones.hpp"
using namespace mdk;

void NativeContacts::bind(Simulation &simulation) {
    NonlocalForce::bind(simulation);

    auto& model = simulation.data<Model>();
    for (auto const& cont: model.contacts) {
        if ((ContactTypeIdx)cont.type != ContactTypeIdx::SSBOND) {
            int i1 = cont.res[0];
            int i2 = cont.res[1];
            if (i1 > i2) std::swap(i1, i2);
            allContacts.emplace_back((Contact) {
                .i1 = i1, .i2 = i2,
                .r_min = cont.dist0
            });
        }
    }

    /* Remove duplicated contacts, this probably should be done in 'coarsen' */ 
    sort(allContacts.begin(), allContacts.end(),
         [](Contact const& a, Contact const& b) -> bool {
             if (a.i1 == b.i1) return a.i2 < b.i2;
             return a.i1 < b.i1;
         });
    allContacts.resize(
        distance(allContacts.begin(),
                 unique(allContacts.begin(), allContacts.end(),
                        [](Contact const& a, Contact const& b) -> bool {
                            return a.i1 == b.i1 && a.i2 == b.i2;
                        })));

    installIntoVL();
}

vl::Spec NativeContacts::spec() const {
    // TODO: make it settable
    double maxCutoff = 18.0 * angstrom;

    return (vl::Spec) {
        .cutoffSq = pow(maxCutoff, 2.0),
        .minBondSep = 3
    };
}

bool operator<(NativeContacts::Contact const& p1, std::pair<int, int> const& p2)  {
    return std::make_pair(p1.i1, p1.i2) < p2;
}

bool operator==(NativeContacts::Contact const& p1, std::pair<int, int> const& p2) {
    return std::make_pair(p1.i1, p1.i2) == p2;
}

void NativeContacts::vlUpdateHook() {
    curPairs.clear();
    newVL.clear();

    auto allContIter = allContacts.begin();
    auto allContEnd = allContacts.end();

    for (auto const& p: vl->pairs) {
        while (allContIter != allContEnd && *allContIter < p)
            ++allContIter;

        if (allContIter != allContEnd && *allContIter == p) {
            curPairs.emplace_back(*allContIter);
        }
        else {
            newVL.emplace_back(p);
        }
    }

    std::swap(vl->pairs, newVL);
}

void NativeContacts::asyncPart(Dynamics &dyn) {
    #pragma omp for nowait 
    for (auto const& cont: curPairs) {
        auto r12 = state->top(state->r[cont.i1] - state->r[cont.i2]);
        auto x2 = r12.squaredNorm();
        if (x2 > savedSpec.cutoffSq) continue;

        auto x = sqrt(x2);
        auto unit = r12/x;

        auto lj = LennardJones(cont.r_min, depth);
        lj.computeF(unit, x, dyn.V, dyn.F[cont.i1], dyn.F[cont.i2]);
    }
}
void NativeContacts::serialize(std::ostream &ostream) {
    ostream << curPairs.size() << '\n';
    for (Contact const &c: curPairs) {
        ostream << c.i1 << ' ' << c.i2 << ' ' << *(reinterpret_cast<const uint64_t*>(&c.r_min)) << '\n';
    }
    ostream << allContacts.size() << '\n';
    for (Contact const &c: allContacts) {
        ostream << c.i1 << ' ' << c.i2 << ' ' << *(reinterpret_cast<const uint64_t*>(&c.r_min)) << '\n';
    }
}

void NativeContacts::deserialize(std::istream &istream) {
    size_t size;
    istream >> size;
    curPairs.resize(size);
    for (Contact &c: curPairs) {
        istream >> c.i1 >> c.i2 >> *(reinterpret_cast<uint64_t*>(&c.r_min));
    }
    istream >> size;
    allContacts.resize(size);
    for (Contact &c: allContacts) {
        istream >> c.i1 >> c.i2 >> *(reinterpret_cast<uint64_t*>(&c.r_min));
    }
}
