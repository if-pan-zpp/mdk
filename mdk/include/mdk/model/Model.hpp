#pragma once
#include <vector>
#include <string>
#include <optional>
#include <Eigen/Core>
#include "../utils/Units.hpp"
#include "../utils/Random.hpp"
#include "../utils/Topology.hpp"
#include "../utils/ResType.hpp"
#include "../utils/ContactType.hpp"
#include "../files/cmap/ContactMap.hpp"

namespace mdk {
    class Model {
    public:
        int n = 0;

        struct Residue;
        struct Chain;

        struct Residue {
            int idx, chainIdx;
            Vector r, v;
            double mass;
            ResType type;
            std::optional<Vector> nat_r;
        };
        std::vector<Residue> residues;
        Residue& addResidue(Chain *chain = nullptr);

        struct Chain {
            int idx;
            int start, end;
            std::vector<int> structuredParts;
        };
        std::vector<Chain> chains;
        Chain& addChain();

        struct Contact {
            int idx;
            int res[2];
            ContactType type;
            double dist0;
        };
        std::vector<Contact> contacts;
        Contact& addContact();

        struct StructuredPart {
            int idx;
            int off, len;
            std::vector<double> angle, dihedral;
        };
        std::vector<StructuredPart> structuredParts;
        StructuredPart& addSP();

        Topology top;

    public:
        void morphIntoLine();
        void morphIntoSAW(Random& rand,
            bool useTop = false,
            double density = 1e-4 * atom / pow(angstrom, 3.0),
            double minDist = 4.56 * angstrom);

        void legacyMorphIntoSAW(Random& rand,
            bool useTop = false,
            double density = 1e-4 * atom / pow(angstrom, 3.0),
            double minDist = 4.56 * angstrom,
            bool nativeBondLen = false);

        void exactLegacySAW(Random& rand,
            bool useTop = false,
            double density = 0.0 * atom / pow(angstrom, 3.0),
            double minDist = 4.56 * angstrom,
            double bond = 3.8 * angstrom);

        void initVelocity(Random& rand,
                          double temperature,
                          bool useMass = true);

        StructuredPart& addContactMap(cmap::ContactMap const& contactMap);
        void addCMapContacts(cmap::ContactMap const& contactMap, Chain& chain);

        void useAverageMasses();

    private:
        std::vector<std::pair<Residue*, Residue*>> nonlocalPairs();
    };
}
