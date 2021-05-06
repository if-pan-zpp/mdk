#pragma once
#include <mdk/cpu/verlet/Factory.hpp>
#include <mdk/cpu/nonlocal/qa/ContactType.hpp>

namespace mdk {
    class System;

    enum class NCType: int8_t {
        NONE, NATIVE, SSBOND, QA_FORMING, QA_BREAKING
    };

    struct NormalData {
        NCType type;
        qa::ContactType qaType;
        double time0, r_min;
    };

    class NormalVL: public vl::List<NormalData> {
    public:
        explicit NormalVL(System const& system);
        double finish() override;
    };
}
