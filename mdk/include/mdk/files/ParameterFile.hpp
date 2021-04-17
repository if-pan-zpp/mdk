#include <filesystem>
#include "utils/AminoAcid.hpp"
#include "files/ContactMapFile.hpp"

namespace mdk {
    class SequenceFile {
    public:
        SequenceFile(std::filesystem::path const& path);

    public:
        struct Chain {
            std::vector<AminoAcid> codes;
            std::vector<ContactMapFile*> contactMaps;
        };

        std::vector<Chain> chains;
        std::unordered_map<std::string, ContactMapFile> contactMaps;
    };
}