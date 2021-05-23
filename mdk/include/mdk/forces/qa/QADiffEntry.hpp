#pragma once
#include <cstdint>
#include <forces/qa/QAState.hpp>
#include <forces/qa/Stat.hpp>

namespace mdk {
    struct QADiffEntry {
        QAContact cont;
        int oldIdx;
        Stat statDiffs[2];
        enum class Request { FORM, BREAK, DESTROY } request;
    };
}