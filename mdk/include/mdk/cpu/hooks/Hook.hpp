#pragma once

namespace mdk {
    class System;

    class Hook {
    public:
        virtual void execute(System& system) = 0;
    };
}
