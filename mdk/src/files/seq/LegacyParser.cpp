#include "files/seq/LegacyParser.hpp"
#include "files/go/LegacyParser.hpp"
#include "utils/Text.hpp"
#include <fstream>
using namespace mdk::seq;
using namespace std;

Sequence LegacyParser::read(const std::filesystem::path &path) {
    auto file = ifstream(path);
    Sequence seq;
    go::LegacyParser goParser;

    string line;
    getline(file, line);

    int numOfChains;
    if (line.substr(0, 7) == "screend") {
        seq.screeningDist = stod(line.substr(8));
        file >> numOfChains;
    }
    else {
        numOfChains = stoi(line);
    }

    seq.chains = vector<Sequence::Chain>(numOfChains);
    for (auto& chain: seq.chains) {
        int numOfResidues;
        file >> numOfResidues;

        chain.codes = vector<AminoAcid>(numOfResidues);
        string codesText;
        file >> codesText;
        for (int i = 0; i < numOfResidues; ++i) {
            chain.codes[i] = (AminoAcid)codesText[i];
        }

        int numOfStructuredParts;
        file >> numOfStructuredParts;

        chain.structuredParts = vector<string>(numOfStructuredParts);
        for (int i = 0; i < numOfStructuredParts; ++i) {
            filesystem::path spPath;
            file >> spPath;
            spPath = path.parent_path() / spPath;

            auto iter = seq.structuredParts.find(spPath);
            if (iter == seq.structuredParts.end()) {
                auto spFile = ifstream(spPath);
                auto sp = goParser.read(spFile);

                auto newNode = make_pair(spPath, move(sp));
                iter = seq.structuredParts.emplace(move(newNode)).first;
            }

            chain.structuredParts[i] = iter->first;
        }
    }

    return seq;
}
