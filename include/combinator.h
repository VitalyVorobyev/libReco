#ifndef __COMBINATOR_H__
#define __COMBINATOR_H__

//#include HEPEVT_H
#include "particle/Particle.h"
#include <vector>

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif

///
/// \brief Class containing methods of particle candidates reconstruction
///
class Combinator {
 public:
    Combinator() {}
    ~Combinator() {}

    static void SetNT(const bool ntuple_flag) {m_tupfl = ntuple_flag;}
    static void SetMC(const bool mc_flag)     {m_mc_flag = mc_flag;}

    static int make_vtx_for_b0tod0pipi(Particle& b0);
    static int make_mass_for_b0tod0pipi(Particle& b0);

// * B0 makers * //
    /// B0 -> D0 pi+ pi-
    static int make_b0tod0pipi(
        std::vector<Particle>& b0l, std::vector<Particle> &d0l,
        std::vector<Particle> &pipl, std::vector<Particle> &piml);
    /// B0 -> D0 pi0 pi0
    static int make_b0tod0pi0pi0(
        std::vector<Particle>& b0l,
        std::vector<Particle> &d0l, std::vector<Particle> &pi0l);
    /// Self-tagging B0 decays: B0 -> D- Ds(*j)+, Ds(*j)K-, ...
    static int make_b0toxy(
        std::vector<Particle>& b0l,
        std::vector<Particle>& xpl, std::vector<Particle>& yml,
        std::vector<Particle>& xml, std::vector<Particle>& ypl,
        const int mode = 0);
// * B+ makers * //
    /// B+ -> X0 Y+
    static int make_bptoxy(
        std::vector<Particle>& bpl,
        std::vector<Particle>& x0l, std::vector<Particle>& ypl,
        const int mode = 0);
// * Particle lists * //
    /// Track lists
    static int make_kpi(
        std::vector<Particle> &pipl, std::vector<Particle> &piml,
        std::vector<Particle> &kpl, std::vector<Particle> &kml);
    /// Ks0 list
    static int make_ks(std::vector<Particle> &ksl);
    /// pi0 list
    static int make_pi0(std::vector<Particle> &pi0l);
    /// gamma list
    static int make_gamma(std::vector<Particle>& gl);

// * Unflavored and strange meson makers * //
    /// eta -> gg
    static int make_etato2g(
        std::vector<Particle>& etal,
        std::vector<Particle>& gammas, const int mode);
    /// eta ->pi+ pi- pi0
    static int make_etato3pi(
        std::vector<Particle>& etal,
        std::vector<Particle>& pipl, std::vector<Particle>& piml,
        std::vector<Particle>& pi0l, const int mode);
    /// omega -> pi+ pi- pi0
    static int make_omega(
        std::vector<Particle>& omegal, std::vector<Particle>& pipl,
        std::vector<Particle>& piml, std::vector<Particle>& pi0l);
    /// Kstar0 -> K- pi+
    static int make_kstar0(
        std::vector<Particle> &kst0l,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,   //      K*0 -> K- pi+
        std::vector<Particle> &kpl, std::vector<Particle> &piml);  // anti-K*0 -> K+ pi-
    /// Kstar0 -> K- pi+, separated flavors
    static int make_kstar0(
        std::vector<Particle> &kst0l, std::vector<Particle> &kst0bl,  // to be filled
        std::vector<Particle> &kml,std::vector<Particle> &pipl,   // K*0 -> K- pi+
        std::vector<Particle> &kpl, std::vector<Particle> &piml);  // anti-K*0 -> K+ pi-
    /// phi -> K+ K-
    static int make_phitokk(
        std::vector<Particle> &phil,
        std::vector<Particle> &kpl, std::vector<Particle> &kml);

// * D0 makers * //
    /// D0 -> Ks0 h+ h-
    static int make_d0tok0shh(
        std::vector<Particle> &d0l,  // to be filled
        std::vector<Particle> &ksl, std::vector<Particle> &hpl,
        std::vector<Particle> &hml, const int mode = 20);
    /// D0 -> K+K-, pi+pi-, Ks0 pi0,  Ks0 eta, Ks0 omega, Ks0 phi
    static int make_d0tohh(
        std::vector<Particle> &d0l,  // to be filled
        std::vector<Particle> &h1l, std::vector<Particle> &h2l,
        const int mode = 21, const bool narrow = false); 
    /// D0 -> K+ pi-
    static int make_d0tokpi(
        std::vector<Particle> &d0l,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D0 -> K- pi+
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // anti-D0 -> K+ pi-
        const int mode = 10);
    /// D0 -> K+ pi-, separated flavors
    static int make_d0tokpi(
        std::vector<Particle> &d0l, std::vector<Particle> &d0bl,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D0 -> K- pi+
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // anti-D0 -> K+ pi-
        const int mode = 10);
    /// D0 -> K+ pi- pi0
    static int make_d0tokpipi0(
        std::vector<Particle> &d0l,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D0 -> K- pi+ pi0
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // anti-D0 -> K+ pi- pi0
        std::vector<Particle> &pi0l,
        const int mode = 11, const bool narrow = false); 
    /// D0 -> K+ pi- pi0, separated flavors
    static int make_d0tokpipi0(
        std::vector<Particle> &d0l, std::vector<Particle> &d0bl,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D0 -> K- pi+ pi0
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // anti-D0 -> K+ pi- pi0
        std::vector<Particle> &pi0l,
        const int mode = 11, const bool narrow = false);
    /// D0 -> K+ pi- pi+ pi-
    static int make_d0tokpipipi(
        std::vector<Particle> &d0l,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D0 -> K- 2pi+ pi-
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // anti-D0 -> K+ 2pi- pi+
        const int mode = 12, const bool narrow = false);
    /// D0 -> K+ pi- pi+ pi-, separated flavors
    static int make_d0tokpipipi(
        std::vector<Particle> &d0l, std::vector<Particle> &d0bl,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D0 -> K- 2pi+ pi-
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // anti-D0 -> K+ 2pi- pi+
        const int mode = 12, const bool narrow = false);
// * D+ makers * //
    /// D+ -> K- 2pi+
    static int make_dptokpipi(
        std::vector<Particle> &dpl,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D+ -> K- 2pi+
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // D- -> K+ 2pi-
        const int mode = 110);
    /// D+ -> K- 2pi+, separated flavors
    static int make_dptokpipi(
        std::vector<Particle> &dpl, std::vector<Particle> &dml,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D+ -> K- 2pi+
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // D- -> K+ 2pi-
        const int mode = 110);
    /// D* -> D {gamma, pi0, pi+}
    static int make_dstar(
        std::vector<Particle> &dstl,  // to be filled
        std::vector<Particle> &dl, std::vector<Particle> &xl);
// * Ds makers * //
    /// Ds+ -> phi pi+, Ks0 K+. The first particle is neutral, the second is charged
    static int make_dstoh0hp(
        std::vector<Particle> &dsl,  // to be filled
        std::vector<Particle> &h0l, std::vector<Particle> &hpl,
        std::vector<Particle> &hml, const int mode = 0);
    /// Ds+ -> phi pi+, Ks0 K+, separated flavors
    static int make_dstoh0hp(
        std::vector<Particle> &dspl, std::vector<Particle> &dsml,  // to be filled
        std::vector<Particle> &h0l, std::vector<Particle> &hpl,
        std::vector<Particle> &hml, const int mode = 0);
    /// Ds+ -> anti-K*0 K+
    static int make_dstoKK(
        std::vector<Particle> &dsl,  // to be filled
        std::vector<Particle> &kst0bl, std::vector<Particle> &kpl,  // Ds+ -> anti-K*0 K+
        std::vector<Particle> &kst0l, std::vector<Particle> &kml,  // Ds- -> K*0 K-
        const int mode = 1);
    /// Ds+ -> anti-K*0 K+, separated flavors
    static int make_dstoKK(
        std::vector<Particle> &dspl, std::vector<Particle> &dsml,  // to fill
        std::vector<Particle> &kst0bl, std::vector<Particle> &kpl,  // Ds+ -> anti-K*0 K+
        std::vector<Particle> &kst0l, std::vector<Particle> &kml,  // Ds- -> K*0 K-
        const int mode = 1);
    /// D*s -> Ds gamma
    static int make_dsstar(
        std::vector<Particle> &dsstl,  // to be filled
        std::vector<Particle> &dsl, std::vector<Particle> &gammal);
    /// D*sj -> D(*)s X
    static int make_dsjtodsx(
        std::vector<Particle> &dsjl, std::vector<Particle> &dspl,
        std::vector<Particle> &dsml, std::vector<Particle> &xl,
        const int mode = 0);
    /// D*sj -> D(*)s X, separated flavors
    static int make_dsjtodsx(
        std::vector<Particle> &dsjpl, std::vector<Particle> &dsjml,
        std::vector<Particle> &dspl, std::vector<Particle> &dsml,
        std::vector<Particle> &xl, const int mode = 0);
    /// D*sj -> D(*)s X1 X2
    static int make_dsjtodsxy(
        std::vector<Particle> &dsjl, std::vector<Particle> &dspl,
        std::vector<Particle> &dsml, std::vector<Particle> &xl,
        std::vector<Particle> &yl, const int mode = 0);
    /// D*sj -> D(*)s X1 X2, separated flavors
    static int make_dsjtodsxy(
        std::vector<Particle> &dsjpl, std::vector<Particle> &dsjml,
        std::vector<Particle> &dspl, std::vector<Particle> &dsml,
        std::vector<Particle> &xl, std::vector<Particle> &yl,
        const int mode = 0);

  /// D** -> D pi+ pi-
    static int make_dststtoxyz(
        std::vector<Particle>& dststl,
        std::vector<Particle>& xl,
        std::vector<Particle>& yl,
        std::vector<Particle>& zl, const int mode);
// * Interface for configuration * //
    /// Minimal photon energy
    static void SetEgammaMin(const double& x) {m_egamma_min = x;}
    /// D0 mass window
    static void Set_md0_range(const double& x, const double& y) {
        m_d0mmin = x; m_d0mmax = y;
    }
    /// K*0 mass window
    static void Set_kst0_range(const double& x, const double& y) {
        m_kst0mmin = x; m_kst0mmax = y;
    }
    /// D*s mass difference window
    static void Set_Dsst_dm_range(const double& x, const double& y) {
        m_dsst_dm_min = x; m_dsst_dm_max = y;
    }
    /// D*sj mass window
    static void Set_Dsj_range(const double& x, const double& y) {
        m_mdsstj_min = x; m_mdsstj_max = y;
    }

 private:
    static void setH0toGGErrors(std::vector<Particle>& etal);
    static void D0MassCut(std::vector<Particle> &v, const bool narrow = false);
    static void D0UIS(std::vector<Particle> &v, const int mode);

    static bool m_tupfl;
    static bool m_mc_flag;

    static double m_d0mmin;
    static double m_d0mmax;
    static double m_d0mmin_narrow;
    static double m_d0mmax_narrow;
    static double m_dpmmin;
    static double m_dpmmax;
    static double m_mphi_min;
    static double m_mphi_max;
    static double m_mds_min;
    static double m_mds_max;
    static double m_omega_min;
    static double m_omega_max;
    static double m_eta_min_ppp;
    static double m_eta_max_ppp;
    static double m_eta_min_gg;
    static double m_eta_max_gg;
    static double m_mdsstj_min;
    static double m_mdsstj_max;

    static double m_egamma_min;
    static double m_egamma_min_pi0;
    static double m_egamma_min_eta;
    static double m_kst0mmin;
    static double m_kst0mmax;
    static double m_dsst_dm_min;
    static double m_dsst_dm_max;
    static double m_dst0_dm_min;
    static double m_dst0_dm_max;
    static double m_dstp_dm_min;
    static double m_dstp_dm_max;
    static double m_mdstst_min;
    static double m_mdstst_max;
};

#if defined(BELLE_NAMESPACE)
}
#endif

#endif  // __COMBINATOR_H__

