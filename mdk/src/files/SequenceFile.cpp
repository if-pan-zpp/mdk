#include "files/SequenceFile.hpp"
#include "utils/Text.hpp"
#include <fstream>
using namespace mdk;
using namespace std;

SequenceFile::SequenceFile(const filesystem::path &path) {
    auto file = ifstream(path);

    string line;
    getline(file, line);

    int numOfChains;
    if (line.substr(0, 7) == "screend") {
        screend = stod(line.substr(8));
        file >> numOfChains;
    }
    else {
        numOfChains = stoi(line);
    }

    chains = vector<Chain>(numOfChains);
    for (auto& chain: chains) {
        int numOfResidues;
        file >> numOfResidues;

        chain.codes = vector<AminoAcid>(numOfResidues);
        string codesText;
        file >> codesText;
        for (int i = 0; i < numOfResidues; ++i) {
            chain.codes[i] = (AminoAcid)codesText[i];
        }

        int numOfContactMaps;
        file >> numOfContactMaps;

        chain.contactMaps = vector<ContactMapFile*>(numOfContactMaps);
        for (int i = 0; i < numOfContactMaps; ++i) {
            filesystem::path contactMapPath;
            file >> contactMapPath;
            contactMapPath = path.parent_path() / contactMapPath;

            auto iter = contactMaps.find(contactMapPath);
            if (iter == contactMaps.end()) {
                auto contactMapFile = ifstream(contactMapPath);
                auto contactMap = ContactMapFile(contactMapFile);
                auto newNode = make_pair(contactMapPath, move(contactMap));
                iter = contactMaps.emplace(move(newNode)).first;
            }

            chain.contactMaps[i] = &iter->second;
        }
    }
}
