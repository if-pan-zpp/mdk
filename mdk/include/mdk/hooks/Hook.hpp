#pragma once

namespace mdk {
    class Hook {
    public:
        virtual void execute(int step_nr) = 0;
    };
}
