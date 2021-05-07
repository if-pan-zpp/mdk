#pragma once
#include <mdk/cpu/generic/Harmonic.hpp>
#include <mdk/cpu/generic/LennardJones.hpp>
#include <mdk/cpu/generic/ShiftedTruncatedLJ.hpp>
#include <variant>

namespace mdk {
    class DisulfideV {
    public:
        double dist0 = 6.0 * angstrom;
        std::variant<Harmonic, LennardJones, ShiftedTruncatedLJ> ff;

        template<typename T>
        T& setVersion() {
            if constexpr (std::is_same_v<T, Harmonic>) {
                ff = Harmonic();
                auto& harm = std::get<Harmonic>(ff);
                return harm;
            }
            else if constexpr (std::is_same_v<T, LennardJones>) {
                ff = LennardJones();
                auto& lj = std::get<LennardJones>(ff);
                lj.r_min = dist0;
                lj.depth = 1.0 * eps;
                return lj
            }
            else if constexpr (std::is_same_v<T, ShiftedTruncatedLJ>) {
                ff = ShiftedTruncatedLJ();
                auto& stlj = std::get<ShiftedTruncatedLJ>(ff);
                stlj.r_min = dist0;
                stlj.depth = 4.0 * eps;
                return stlj;
            }
        }

        void kernel(double norm, double& V, double& dV_dn) const;
        void asForce(VRef unit, double norm, double& V,
            Vector& dV_dn1, Vector& dV_dn2) const;
    };

    void DisulfideV::kernel(double norm, double &V, double &dV_dn) const {
        if (std::holds_alternative<Harmonic>(ff)) {
            std::get<Harmonic>(ff).kernel(norm - dist0, V, dV_dn);
        }
        else if (std::holds_alternative<LennardJones>(ff)) {
            std::get<LennardJones>(ff).kernel(norm, V, dV_dn);
        }
        else {
            std::get<ShiftedTruncatedLJ>(ff).kernel(norm, V, dV_dn);
        }
    }

    void DisulfideV::asForce(VRef unit, double norm, double &V, Vector &dV_dn1,
            Vector &dV_dn2) const {
        if (std::holds_alternative<Harmonic>(ff)) {
            std::get<Harmonic>(ff).asForce(unit, norm - dist0, V, dV_dn1, dV_dn2);
        }
        else if (std::holds_alternative<LennardJones>(ff)) {
            std::get<LennardJones>(ff).asForce(unit, norm, V, dV_dn1, dV_dn2);
        }
        else {
            std::get<ShiftedTruncatedLJ>(ff).asForce(unit, norm, V, dV_dn1, dV_dn2);
        }
    }
}