#include "forces/PseudoImproperDihedral.hpp"
#include "simul/Simulation.hpp"
using namespace mdk;

bool LambdaPeak::supp(double psi) const {
    double s = alpha * (psi - psi0);
    return -M_PI < s && s < M_PI;
}

void LambdaPeak::eval(double psi, double &L, double &dL_dpsi) const {
    double s = alpha * (psi - psi0);
    if (cosineVersion) {
        L = 0.5 * cos(s) + 0.5;
        dL_dpsi = -0.5 * alpha * sin(s);
    }
    else {
        double t = s/M_PI;
        auto x_inv = 1.0/(2.0*t*t-2.0*abs(t)+1.0);
        L = (t*t-2.0*abs(t)+1.0) * x_inv;
        if (t > 0) {
            double g = 2.0*t*(t-1.0);
            dL_dpsi = alpha * g/((g+1.0)*(g+1.0)) / M_PI;
        }
        else {
            double g = -2.0*t*(t+1.0);
            dL_dpsi = alpha * g/((g-1.0)*(g-1.0)) / M_PI;
        }
    }
}

template<typename LJ>
void perLambda(LambdaPeak *lambda[2], LJ const& lj, double psi[2],
               double norm, double& A, double& B, double& C, double scale) {
    if (lambda[0]->supp(psi[0]) && lambda[1]->supp(psi[1])) {
        double L[2], dL_dpsi[2];
        for (int m = 0; m < 2; ++m)
            lambda[m]->eval(psi[m], L[m], dL_dpsi[m]);

        double ljV = 0.0, dljV_dn = 0.0;
        lj.kernel(norm, ljV, dljV_dn);

        A += dL_dpsi[0] * L[1] * ljV * scale;
        B += dL_dpsi[1] * L[0] * ljV * scale;
        C += L[0] * L[1] * dljV_dn * scale;
    }
}

void PseudoImproperDihedral::deriveAngles(int _i1, int _i2, double *psi,
                                          Vector (*dpsi_dr)[4]) const {

    int idx[2] = { _i1, _i2 };
    for (int m = 0; m < 2; ++m) {
        for (int ix = 0; ix < 4; ++ix) {
            dpsi_dr[m][ix].setZero();
        }
    }

    for (int m = 0; m < 2; ++m) {
        int i1 = idx[m], i2 = idx[m]+1, i3 = idx[m]-1;

        Vector ri = state->r[i1],
               rj = state->r[i2],
               rk = state->r[i3],
               rl = state->r[idx[1-m]];

        auto rij = ri - rj, rkj = rk - rj, rkl = rk - rl;
        Vector rm = rij.cross(rkj);
        double rm_norm = rm.norm();
        Vector rn = rkj.cross(rkl);
        double rn_norm = rn.norm();

        //TODO: do something sensible when rm or rn norm is too close to zero

        double rkj_norm = rkj.norm();
        Vector fi = rm * rkj_norm / rm_norm / rm_norm;
        Vector fl = -rn * rkj_norm / rn_norm / rn_norm;
        Vector df = (fi * rij.dot(rkj) - fl * rkl.dot(rkj)) / (rkj_norm * rkj_norm);
        Vector fj = -fi + df;
        Vector fk = -fl - df;

        dpsi_dr[m][0] = fi;
        dpsi_dr[m][1] = fj;
        dpsi_dr[m][2] = fk;
        dpsi_dr[m][3] = fl;

        double cos_psi = rm.dot(rn) / (rm_norm * rn_norm);
        psi[m] = acos(cos_psi);
        if (rij.dot(rn) < 0.0) psi[m] = -psi[m];
    }
}

void PseudoImproperDihedral::bind(Simulation &simulation) {
    NonlocalForce::bind(simulation);

    types = &simulation.data<Types>();
    seqs = &simulation.data<Chains>();

    bb_neg_lj.r_min = 6.2 * angstrom;
    bb_neg_lj.depth = 1.0 * eps;
    bb_pos_lj.r_min = 5.6 * angstrom;
    bb_pos_lj.depth = 1.0 * eps;

    bb_pos.alpha = 6.4; bb_pos.psi0 = 1.05 * rad;
    bb_neg.alpha = 6.0; bb_neg.psi0 = -1.44 * rad;
    ss.alpha = 1.2; ss.psi0 = -0.23 * rad;

    auto& params = simulation.data<param::Parameters>();
    for (auto const& type1: AminoAcid::aminoAcids()) {
        auto code1 = (int8_t)type1;
        for (auto const& type2: AminoAcid::aminoAcids()) {
            auto code2 = (int8_t)type2;

            auto& lj = ss_ljs[code1][code2];
            lj.depth = (use_MJ && params.mjMatrix)
                ? params.mjMatrix->at({type1, type2})
                : 1.0 * eps;
            lj.sink_max = params.pairwiseMinDist.at({type1, type2});
        }
    }

    installIntoVL();
}

vl::Spec PseudoImproperDihedral::spec() const {
    double cutoff = 18.0 * angstrom;

    return (vl::Spec) {
        .cutoffSq = pow(cutoff, 2.0),
        .minBondSep = 3,
    };
}

void PseudoImproperDihedral::asyncPart(Dynamics &dyn) {
    #pragma omp for nowait
    for (auto const& [i1, i2]: pairs) {
        auto r12 = state->top(state->r[i1] - state->r[i2]);
        auto r12_normsq = r12.squaredNorm();
        if (r12_normsq >= savedSpec.cutoffSq) continue;

        auto type1 = (int8_t)(*types)[i1], type2 = (int8_t)(*types)[i2];
        if (type1 == ResTypeIdx::PRO or type2 == ResTypeIdx::PRO) continue;

        auto norm = sqrt(r12_normsq);
        auto unit = r12 / norm;

        double psi[2];
        Vector dpsi_dr[2][4];
        deriveAngles(i1, i2, psi, dpsi_dr);

        double A = 0.0, B = 0.0, C = 0.0;

        LambdaPeak *peak[2];
        LennardJones *lj = &bb_pos_lj;

        peak[0] = peak[1] = &bb_pos;

        if (!peak[0]->supp(psi[0])) {
            peak[0] = &bb_neg;
        }
        if (!peak[1]->supp(psi[1])) {
            peak[1] = &bb_neg;
            lj = &bb_neg_lj;
        }

        if (i2 - i1 == 3) {
            lj = &bb_neg_lj;
            peak[0] = &bb_pos;
            peak[1] = &bb_neg;
        }
        
        perLambda(peak, *lj, psi, norm, A, B, C, 0.2);

        peak[0] = peak[1] = &ss;
        perLambda(peak, ss_ljs[type1][type2],
                  psi, norm, A, B, C, 1.0);

        int idx_A[4] = { i1, i1+1, i1-1, i2 };
        int idx_B[4] = { i2, i2+1, i2-1, i1 };
        for (int i = 0; i < 4; ++i) {
            dyn.F[idx_A[i]] -= A * dpsi_dr[0][i];
            dyn.F[idx_B[i]] -= B * dpsi_dr[1][i];
        }
        dyn.F[i1] -= C * unit;
        dyn.F[i2] += C * unit;
    }
}

void PseudoImproperDihedral::vlUpdateHook() {
    pairs.clear();
    for (auto const& [i1, i2]: vl->pairs) {
        if (not include_i_4 && seqs->sepByN(i1, i2, 4))
            continue;
        if (seqs->isTerminal[i1] || seqs->isTerminal[i2])
            continue;
        if (not (seqs->sepByAtLeastN(i1, i2, 3)))
            continue;

        pairs.emplace_back(i1, i2);
    }
}

