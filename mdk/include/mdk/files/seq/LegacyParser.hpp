#include <filesystem>
#include <optional>
#include "mdk/utils/AminoAcid.hpp"
#include "files/seq/Data.hpp"

namespace mdk::seq {
    class LegacyParser {
    public:
        Data read(std::filesystem::path const& path);
        void write(std::filesystem::path const& path, Data const& seq);
    };
}