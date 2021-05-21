#pragma once
#include <cstdint>
#include <mdk/cpu/forces/qa/QAState.hpp>
#include <mdk/cpu/forces/qa/Stat.hpp>

namespace mdk {
    struct QADiffEntry {
        QAContact cont;
        int oldIdx;
        Stat statDiffs[2];
        enum class Request { FORM, BREAK, DESTROY } request;
    };
}