#pragma once
#include "../Force.hpp"
#include "../../data/Primitives.hpp"
#include "ComplexNativeDihedral.hpp"
#include "SimpleNativeDihedral.hpp"
#include "HeuresticDihedral.hpp"

namespace mdk {
    class DihedralAngles: public Force {
    private:
        friend class ComplexNativeDihedral;
        ComplexNativeDihedral *compNatDih = nullptr;

        friend class SimpleNativeDihedral;
        SimpleNativeDihedral *simpNatDih = nullptr;

        friend class HeuresticDihedral;
        HeuresticDihedral *heurDih = nullptr;

        Bytes inRange;

    public:
        void bind(Simulation& simulation) override;
        void asyncPart() override;
    };
}