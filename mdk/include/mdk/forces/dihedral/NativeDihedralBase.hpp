#pragma once
#include "DihedralBase.hpp"
#include "../../data/DihedralRanges.hpp"
#include "../../simul/Simulation.hpp"

namespace mdk {
    template<typename Impl>
    class NativeDihedralBase: public DihedralBase<Impl> {
    protected:
        Scalars phi0;

    public:
        void bind(Simulation& simul) {
            using namespace boost::icl;
            this->ranges = simul.data<DihedralRanges>()->native;

            auto& model = *simul.data<Model>();

            phi0 = Scalars(model.n);
            for (auto const& ch: model.chains) {
                for (auto const& spIdx: ch.structuredParts) {
                    auto const& sp = model.structuredParts[spIdx];

                    auto start = ch.start + sp.off;
                    auto end = start + sp.len;
                    auto intv = interval<int>::right_open(start, end);
                    this->ranges.add(intv);

                    for (int i = start; i < end; ++i) {
                        phi0[i] = sp.dihedral[i - start];
                    }
                }
            }
        }
    };
}