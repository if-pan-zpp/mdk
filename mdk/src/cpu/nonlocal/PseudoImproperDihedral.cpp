#include "cpu/nonlocal/PseudoImproperDihedral.hpp"
#include "cpu/generic/LennardJones.hpp"
using namespace mdk;

bool LambdaPeak::support(double psi) const {
    double s = alpha * (psi - psi0);
    return -M_PI <= s && s <= M_PI;
}

void LambdaPeak::compute(double psi, double &lam, double &grad) const {
    double s = alpha * (psi - psi0);
    if (cosineVersion) {
        lam = 0.5 * cos(s) + 0.5;
        grad = -0.5 * sin(s);
    }
    else {
        s /= M_PI;
        double bot = 2 * s * s - 2 * abs(s) + 1;
        lam = 1 - (s * s) / bot;
        double top = 2 * s * s - 2 * abs(s);
        grad = (s < 0 ? -1 : 1) * (top / (bot * bot));
    }
}

PseudoImproperDihedral::PseudoImproperDihedral(const Model &model,
        const param::Parameters &params) {
    useMJ = params.mjMatrix.has_value();
    if (useMJ) {
        for (auto const& [p, depth]: params.mjMatrix.value()) {
            auto type1 = (int8_t)p.first, type2 = (int8_t)p.second;
            ss_depth[type1][type2] = ss_depth[type2][type1] = depth;
        }
    }

    for (auto const& [p, r]: params.pairwiseMinDist) {
        auto type1 = (int8_t)p.first, type2 = (int8_t)p.second;
        ss_r_min[type1][type2] = ss_r_min[type2][type1] = r;
    }

    for (int i = 0; i < model.n; ++i) {
        types[i] = (int8_t)model.residues[i].type;
    }
}

void PseudoImproperDihedral::computeAngles(int i, int j, VRef off,
        State const& state, double psi[2], Vector dpsi_dr[2][4]) const {
    int idx[2] = { i, j };
    for (int m = 0; m < 2; ++m) {
        for (int ix = 0; ix < 4; ++ix) {
            dpsi_dr[m][ix].setZero();
        }
    }

    for (int m = 0; m < 2; ++m) {
        int i1 = idx[m]-1, i2 = idx[m], i3 = idx[m];

        Vector r1 = state.r[i1], r2 = state.r[i2], r3 = state.r[i3];
        Vector r12 = r2 - r1, r23 = r3 - r2, r13 = r3 - r1, r14 = r12 + off;

        Vector rij = -r23, rkj = -r13, rkl = -r14;
        Vector rm = -r12.cross(r23);
        double rm_norm = rm.norm();
        Vector rn = r13.cross(r14);
        double rn_norm = rn.norm();

        if (rm_norm == 0.0 || rn_norm == 0.0)
            continue;

        double rkj_norm = rkj.norm();
        Vector fi = rm * rkj_norm / rm_norm;
        Vector fl = - rn * rkj_norm / rn_norm;
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

void PseudoImproperDihedral::perLambdaPeak(const LambdaPeak &peak, double depth,
        double r_min, VRef unit, double norm, double *psi, Vector (*dpsi_dr)[4],
        double &V, Vector *dV_dr) const {
    if (peak.support(psi[0]) && peak.support(psi[1])) {
        double lam[2], lamGrad[2];
        peak.compute(psi[0], lam[0], lamGrad[0]);
        peak.compute(psi[1], lam[1], lamGrad[1]);

        LennardJones lj;
        lj.depth = depth;
        lj.rmin = r_min;

        Vector ljGrad[2];
        ljGrad[0].setZero();
        ljGrad[1].setZero();
        double ljV = 0.0;
        lj.perPair(unit, norm, ljV, ljGrad[0], ljGrad[1]);

        V += lam[0] * lam[1] * ljV;

        for (int m = 0; m < 2; ++m) {
            int ix[4] = {3*m, 3*m+1, 3*m+2, 3*(1-m)+1};
            for (int i = 0; i < 4; ++i) {
                dV_dr[ix[i]] += lamGrad[m] * dpsi_dr[m][ix[i]] * lam[1-m] * ljV;
            }
            dV_dr[ix[0]] += lam[0] * lam[1] * ljGrad[m];
        }
    }
}

void PseudoImproperDihedral::perPair(int i, int j, VRef off, VRef unit,
        double norm, const State &state, StateDiff &sd) const {
    double psi[2];
    Vector dpsi_dr[2][4];
    computeAngles(i, j, off, state, psi, dpsi_dr);

    Vector dV_dr[6];
    for (int ix = 0; ix < 6; ++ix)
        dV_dr[ix].setZero();

    int8_t type1 = types[i], type2 = types[j];
    double cur_depth_ss = useMJ ? ss_depth[type1][type2] : ss_depth0;
    perLambdaPeak(ss, cur_depth_ss, ss_r_min[type1][type2],
        unit, norm, psi, dpsi_dr,
        sd.V, dV_dr);

    perLambdaPeak(bb_pos, bb_pos_depth, bb_pos_r_min,
        unit, norm, psi, dpsi_dr,
        sd.V, dV_dr);

    perLambdaPeak(bb_neg, bb_neg_depth, bb_neg_r_min,
        unit, norm, psi, dpsi_dr,
        sd.V, dV_dr);

    sd.dV_dr[i-1] += dV_dr[0];
    sd.dV_dr[i] += dV_dr[1];
    sd.dV_dr[i+1] += dV_dr[2];

    sd.dV_dr[j-1] += dV_dr[3];
    sd.dV_dr[j] += dV_dr[4];
    sd.dV_dr[j+1] += dV_dr[5];
}