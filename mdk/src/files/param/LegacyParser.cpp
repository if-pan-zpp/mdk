#include "files/param/LegacyParser.hpp"
#include "utils/Text.hpp"
#include <sstream>
using namespace mdk;
using namespace mdk::param;
using namespace std;

static void fetchDefAngleParams(istream &is, Data &data) {
    skipLine(is);
    auto ss = lineStream(is);
    for (auto& coeff: data.defAngleParams)
        ss >> coeff;
}

static void fetchAngleParams(istream &is, Data &data) {
    skipLine(is);
    for (auto var: variants()) {
        auto ss = lineStream(is);
        for (auto& coeff: data.angleParams[var])
            ss >> coeff;
    }
}

static void fetchDihedralParams(istream &is, Data &data) {
    skipLine(is);
    for (auto var: variants()) {
        auto ss = lineStream(is);
        for (auto& coeff: data.dihedralParams[var])
            ss >> coeff;
    }
}

static void fetchSpecificity(istream &is, Data &data) {
    auto ss = lineStream(is);
    vector<AminoAcid> order;
    for (string name; ss >> name; ) {
        auto acid = (AminoAcid)(string)name;
        order.push_back(acid);
    }

    ss = lineStream(is);
    for (auto const& acid: order)
        ss >> (int&)data.specificity[acid].polarization;

    ss = lineStream(is);
    for (auto const& acid: order)
        ss >> data.specificity[acid].coordNum;

    ss = lineStream(is);
    for (auto const& acid: order)
        ss >> data.specificity[acid].hydrophobicCoordNum;

    ss = lineStream(is);
    for (auto const& acid: order)
        ss >> data.specificity[acid].polarCoordNum;
}

static void fetchRadii(istream &is, Data &data) {
    skipLine(is);

    auto ss = lineStream(is);
    for (auto const& acid: AminoAcid::allAminoAcids)
        ss >> data.radius[acid];
}

static void fetchPairwiseData(istream &is, Data &data) {
    string header;
    getline(is, header);
    bool fetchMJ = (header == "amino acid pair distances and energies");
    data.withMJ = fetchMJ;

    int numPairs = AminoAcid::numAminoAcids * (AminoAcid::numAminoAcids+1) / 2;
    for (int i = 0; i < numPairs; ++i) {
        auto ss = lineStream(is);

        string name1, name2;
        ss >> name1 >> name2;
        AminoAcid acid1(name1), acid2(name2);

        double minDist;
        ss >> minDist;
        data.pairwiseMinDist[{acid1, acid2}] = minDist;
        data.pairwiseMinDist[{acid2, acid1}] = minDist;

        if (fetchMJ) {
            double mjEnergy;
            ss >> mjEnergy;

            data.mjMatrix[{acid1, acid2}] = mjEnergy;
            data.mjMatrix[{acid2, acid1}] = mjEnergy;
        }
    }
}

Data LegacyParser::read(istream &is) {
    Data data;

    fetchDefAngleParams(is, data);
    fetchAngleParams(is, data);
    fetchDihedralParams(is, data);
    fetchSpecificity(is, data);
    fetchRadii(is, data);
    fetchPairwiseData(is, data);

    return data;
}

std::ostream &LegacyParser::write(ostream &os, const Data &data) {
    // TODO: writing out parameters in legacy format
    // Priority: low
    return os;
}
