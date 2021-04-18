#include <filesystem>
#include <optional>
#include "mdk/utils/AminoAcid.hpp"
#include "mdk/files/ContactMapFile.hpp"

namespace mdk {
    class SequenceFile {
    public:
        SequenceFile() = default;
        SequenceFile(std::filesystem::path const& path);

    public:
        struct Chain {
            std::vector<AminoAcid> codes;
            std::vector<ContactMapFile*> contactMaps;
        };

        std::optional<double> screend;
        std::vector<Chain> chains;
        std::unordered_map<std::string, ContactMapFile> contactMaps;
    };
}