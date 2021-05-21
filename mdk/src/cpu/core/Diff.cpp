#include <cpu/core/Diff.hpp>
using namespace mdk;

void Diff::init(const State &state) {
    F = Vectors(state.n);
}

void Diff::reset() {
    V = 0;
    F.colwise() = Vector::Zero();
    qaDiffs.clear();
}

Diff& Diff::reduce(std::vector<Diff> &diffs) {
    auto& sum = diffs[0];
    for (int i = 1; i < diffs.size(); ++i) {
        auto& cur = diffs[i];
        sum.F += cur.F;
        sum.V += cur.V;
        sum.qaDiffs.insert(sum.qaDiffs.end(),
            cur.qaDiffs.begin(), cur.qaDiffs.end());
    }
    return sum;
}