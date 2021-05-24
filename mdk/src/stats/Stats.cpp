#include "stats/Stats.hpp"
#include "simul/Simulation.hpp"
#include "runtime/Lambda.hpp"
using namespace mdk;

void Stats::bind(Simulation &simulation) {
    auto& params = simulation.data<param::Parameters>();
    auto& model = simulation.data<Model>();

    stats = std::vector<Stat>(model.n);
    used = false;

    for (int i = 0; i < model.n; ++i) {
        auto& stat = stats[i];
        auto type = model.residues[i].type;
        auto& specificity = params.specificity.at(AminoAcid((int8_t)type));

        stat.backbone =  (ResTypeIdx)type != ResTypeIdx::PRO ? 2 : 1;
        stat.sidechain = specificity.maxSidechain;
        stat.hydrophobicSS = specificity.maxHydrophobicSS;
        stat.polarSS = specificity.maxPolarSS;
    }
}

std::vector<std::unique_ptr<Task>> Stats::tasks() {
    auto zeroDiffs = [&]() -> {
        statDiffs.clear();
    };
    auto zeroDiffsTask = Lambda({}, zeroDiffs, {diffsReset}).unique();

    auto updateStats = [&]() -> {
        for (auto& statDiff: statDiffs) {
            auto res1 = stats[statDiff.i1] + statDiff.diffs[0];
            auto res2 = stats[statDiff.i2] + statDiff.diffs[1];

            if (res1.valid() && res2.valid()) {
                statDiff.status = StatDiff::Status::ACCEPTED;
                stats[statDiff.i1] = res1;
                stats[statDiff.i2] = res2;
            }
            else {
                statDiff.status = StatDiff::Status::REJECTED;
            }
        }
    };
    auto updateStatsTask = Lambda({diffsAdded}, updateStats, {statsUpdated}).unique();

    return { std::move(zeroDiffsTask), std::move(updateStatsTask) };
}
