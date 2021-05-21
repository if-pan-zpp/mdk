#pragma once
#include <mdk/cpu/forces/dihedral/DihedralBase.hpp>
#include <mdk/cpu/data/DihedralRanges.hpp>

namespace mdk {
    template<typename Impl>
    class NativeDihedralBase: public DihedralBase<Impl> {
    protected:
        Scalars phi0;

    public:
        explicit NativeDihedralBase(Model const& model) {
            using namespace boost::icl;
            phi0 = Scalars(model.n);
            this->ranges = DihedralRanges(model).native;

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