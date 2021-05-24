#pragma once
#include <cstdint>
#include "QAState.hpp"
#include "../../stats/Stat.hpp"

namespace mdk {
    struct QADiffEntry {
        QAContact cont;
        int oldIdx;
        Stat statDiffs[2];
        enum class Request: int8_t { FORM, BREAK, DESTROY } request;
    };
}