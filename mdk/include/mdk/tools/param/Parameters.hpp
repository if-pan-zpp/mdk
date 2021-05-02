#pragma once
#include <vector>
#include <unordered_map>
#include <optional>
#include <mdk/utils/AminoAcid.hpp>
#include <mdk/utils/TupleHash.hpp>
#include <mdk/utils/PairType.hpp>

namespace mdk::param {
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
            enum class Polarization: int8_t {
                GLY_PRO, HYDROPHOBIC, POLAR, POS, NEG
            };
            Polarization polarization;
            int coordNum, hydrophobicCoordNum, polarCoordNum;
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