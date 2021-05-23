#include <filesystem>
#include <optional>
#include "../../utils/AminoAcid.hpp"
#include "Sequence.hpp"

namespace mdk::seq {
    class LegacyParser {
    public:
        Sequence read(std::filesystem::path const& path);
        void write(std::filesystem::path const& path, Sequence const& seq);
    };
}