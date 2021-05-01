#include "tools/tab/LegacyParser.hpp"
#include <sstream>
#include "utils/Units.hpp"
using namespace mdk::tab;
using namespace std;

TabEnergy LegacyParser::read(istream &is) {
    TabEnergy tabEnergy;
    auto variants = param::variants();

    for (auto const& variant: variants) {
        auto& angleTab = tabEnergy.angleV[variant];
        angleTab.a = 0;
        angleTab.b = 180.0 * degree;

        auto& dihedralTab = tabEnergy.dihedralV[variant];
        dihedralTab.a = 0;
        dihedralTab.b = 180.0 * degree;
    }

    for (string line; getline(is, line); ) {
        stringstream ss;
        ss << line;

        for (auto const& variant: variants) {
            double v; ss >> v;
            v *= eps;
            tabEnergy.angleV[variant].values.push_back(v);
        }

        for (auto const& variant: variants) {
            double v; ss >> v;
            v *= eps;
            tabEnergy.dihedralV[variant].values.push_back(v);
        }
    }

    return tabEnergy;
}