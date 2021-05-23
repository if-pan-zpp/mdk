#pragma once
#include <vector>
#include <unordered_map>
#include <optional>
#include "../../utils/AminoAcid.hpp"
#include "../../utils/TupleHash.hpp"
#include "../../utils/PairType.hpp"

namespace mdk::param {
    enum class Polarization: int8_t {
        GLY_PRO, HYDROPHOBIC, MISSING,
        POLAR, POLAR_NEG, POLAR_POS
    };

    class Parameters {
    public:
        Parameters();

        using Coeffs = std::vector<double>;
        Coeffs defAngleParams;

        template<typename T>
        using PerPTData = std::unordered_map<PairType, T>;

        PerPTData<Coeffs> angleParams;
        PerPTData<Coeffs> dihedralParams;

        struct SpecificityParams {
            Polarization polarization;
            int maxSidechain, maxHydrophobicSS, maxPolarSS;
        };

        template<typename T>
        using PerAcidData = std::unordered_map<AminoAcid, T>;

        PerAcidData<SpecificityParams> specificity;
        PerAcidData<double> radius;

        template<typename T>
        using PerPairData = std::unordered_map<
            std::pair<AminoAcid, AminoAcid>, T>;

        PerPairData<double> pairwiseMinDist;
        std::optional<PerPairData<double>> mjMatrix;
    };
}