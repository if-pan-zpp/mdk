#pragma once
#include "../Force.hpp"
#include "../../data/Primitives.hpp"
#include "HeuresticBA.hpp"
#include "NativeBA.hpp"
#include <optional>

namespace mdk {
    class BondAngles: public Force {
    private:
        friend class HeuresticBA;
        HeuresticBA* heurBA = nullptr;

        friend class NativeBA;
        NativeBA* natBA = nullptr;

        Bytes inRange;

    public:
        void bind(Simulation &simulation) override;
        void asyncPart() override;
    };
}