#pragma once
#include <vector>
#include <string>
#include <optional>
#include <Eigen/Core>
#include <mdk/utils/Units.hpp>
#include <mdk/utils/Random.hpp>
#include <mdk/utils/Topology.hpp>
#include <mdk/tools/cmap/ContactMap.hpp>

namespace mdk {
    class Model {
    public:
        int n = 0;

        struct Residue;
        struct Chain;

        struct Residue {
            int idx, chainIdx;
            Eigen::Vector3d pos;
            std::string type;
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
            std::string type;
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

        StructuredPart& addContactMap(cmap::ContactMap const& contactMap);
        void addCMapContacts(cmap::ContactMap const& contactMap, Chain& chain);

    private:
        std::vector<std::pair<Residue*, Residue*>> nonlocalPairs();
    };
}
