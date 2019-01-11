#ifndef __COMBINATOR_H__
#define __COMBINATOR_H__

#include "particle/Particle.h"

#include <vector>
#include <map>
#include <string>
#include <array>

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif

typedef std::map<std::string, std::vector<Particle>> PLM;

///
/// \brief Class containing methods of particle candidates reconstruction
///
class Combinator {
    /// Do not instantiate me
    Combinator() {}

    static bool m_tupfl;
    static bool m_mc_flag;
    // Selection parameters
    /// Symmetric cuts
    static std::map<std::string, std::vector<double>> cutSym;
    /// Asymmetric cuts
    static std::map<std::string, std::vector<double>> cutAsym;
    /// Other parameters 
    static std::map<std::string, double> par;

    static void massCutSym(std::vector<Particle> &v, const std::string& key);
    static void massCutAsym(std::vector<Particle> &v, const std::string& key);
    static void dmassCutSym(std::vector<Particle> &v, const std::string& key);

 public:
    static void init();

    static void SetNT(bool ntuple_flag) {m_tupfl = ntuple_flag;}
    static void SetMC(bool mc_flag)     {m_mc_flag = mc_flag;}

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
        int mode = 0);
// * B+ makers * //
    /// B+ -> X0 Y+
    static int make_bptoxy(
        std::vector<Particle>& bpl,
        std::vector<Particle>& x0l, std::vector<Particle>& ypl,
        int mode = 0);
// * Particle lists * //
    /// Track lists
    static int make_kpi(PLM& plm);
    /// Track lists
    static int make_kpi(
        std::vector<Particle> &pipl, std::vector<Particle> &piml,
        std::vector<Particle> &kpl, std::vector<Particle> &kml);
    /// Ks0 list
    static int make_ks(PLM& plm);
    static int make_ks(std::vector<Particle> &ksl);
    /// pi0 list
    static int make_pi0(PLM& plm);
    static int make_pi0(std::vector<Particle> &pi0l);
    /// gamma list
    static int make_gamma(PLM& plm);
    static int make_gamma(std::vector<Particle>& gl);

// * Unflavored and strange meson makers * //
    /// eta -> gg
    static int make_etato2g(
        std::vector<Particle>& etal,
        std::vector<Particle>& gammas, int mode);
    /// eta ->pi+ pi- pi0
    static int make_etato3pi(
        std::vector<Particle>& etal,
        std::vector<Particle>& pipl, std::vector<Particle>& piml,
        std::vector<Particle>& pi0l, int mode);
    /// omega -> pi+ pi- pi0
    static int make_omega(
        std::vector<Particle>& omegal, std::vector<Particle>& pipl,
        std::vector<Particle>& piml, std::vector<Particle>& pi0l);
    /// Kstar0 -> K- pi+
    static int make_kstar0(PLM& plm, bool separate);
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
    static int make_phitokk(PLM& plm);
    static int make_phitokk(
        std::vector<Particle> &phil,
        std::vector<Particle> &kpl, std::vector<Particle> &kml);

// * D0 makers * //
    /// D0 -> Ks0 h+ h-
//    static int make_d0tok0shh(PLM& plm, int mode = 20);
    /// D0 -> K+K-, pi+pi-, Ks0 pi0,  Ks0 eta, Ks0 omega, Ks0 phi
//    static int make_d0tohh(PLM& plm, int mode = 21, bool narrow = false);
    /// D0 -> K+ pi-
//    static int make_d0tokpi(PLM& plm, bool separate, int mode = 10);
    /// D0 -> K+ pi- pi0
//    static int make_d0tokpipi0(PLM& plm, bool separate, int mode = 11);
    /// D0 -> K+ pi- pi+ pi-
//    static int make_d0tok3pi(PLM& plm, bool separate, int mode = 12);

    /// D0 -> Ks0 h+ h-
    static int make_d0tok0shh(
        std::vector<Particle> &d0l,  // to be filled
        std::vector<Particle> &ksl, std::vector<Particle> &hpl,
        std::vector<Particle> &hml, int mode = 20);
    /// D0 -> K+K-, pi+pi-, Ks0 pi0,  Ks0 eta, Ks0 omega, Ks0 phi
    static int make_d0tohh(
        std::vector<Particle> &d0l,  // to be filled
        std::vector<Particle> &h1l, std::vector<Particle> &h2l,
        int mode = 21);
    /// D0 -> K+ pi-
    static int make_d0tokpi(
        std::vector<Particle> &d0l,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D0 -> K- pi+
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // anti-D0 -> K+ pi-
        int mode = 10);
    /// D0 -> K+ pi-, separated flavors
    static int make_d0tokpi(
        std::vector<Particle> &d0l, std::vector<Particle> &d0bl,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D0 -> K- pi+
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // anti-D0 -> K+ pi-
        int mode = 10);
    /// D0 -> K+ pi- pi0
    static int make_d0tokpipi0(
        std::vector<Particle> &d0l,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D0 -> K- pi+ pi0
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // anti-D0 -> K+ pi- pi0
        std::vector<Particle> &pi0l,
        int mode = 11, bool narrow = false); 
    /// D0 -> K+ pi- pi0, separated flavors
    static int make_d0tokpipi0(
        std::vector<Particle> &d0l, std::vector<Particle> &d0bl,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D0 -> K- pi+ pi0
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // anti-D0 -> K+ pi- pi0
        std::vector<Particle> &pi0l,
        int mode = 11, bool narrow = false);
    /// D0 -> K+ pi- pi+ pi-
    static int make_d0tokpipipi(
        std::vector<Particle> &d0l,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D0 -> K- 2pi+ pi-
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // anti-D0 -> K+ 2pi- pi+
        int mode = 12, bool narrow = false);
    /// D0 -> K+ pi- pi+ pi-, separated flavors
    static int make_d0tokpipipi(
        std::vector<Particle> &d0l, std::vector<Particle> &d0bl,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D0 -> K- 2pi+ pi-
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // anti-D0 -> K+ 2pi- pi+
        int mode = 12, bool narrow = false);
// * D+ makers * //
    /// D+ -> K- 2pi+
    static int make_dptokpipi(
        std::vector<Particle> &dpl,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D+ -> K- 2pi+
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // D- -> K+ 2pi-
        int mode = 110);
    /// D+ -> K- 2pi+, separated flavors
    static int make_dptokpipi(
        std::vector<Particle> &dpl, std::vector<Particle> &dml,  // to be filled
        std::vector<Particle> &kml, std::vector<Particle> &pipl,  // D+ -> K- 2pi+
        std::vector<Particle> &kpl, std::vector<Particle> &piml,  // D- -> K+ 2pi-
        int mode = 110);
    /// D* -> D {gamma, pi0, pi+}
    static int make_dstar(
        std::vector<Particle> &dstl,  // to be filled
        std::vector<Particle> &dl, std::vector<Particle> &xl);

// * Ds makers * //
    /// Ds+ -> phi pi+
    static int make_dstophipi(PLM& plm, bool separate, int mode = 0);
    /// Ds+ -> K*0 K+
    static int make_dstokstk(PLM& plm, bool separate, int mode = 1);
    /// Ds+ -> Ks0 K+
    static int make_dstoks0k(PLM& plm, bool separate, int mode = 2);

    /// Ds+ -> phi pi+, Ks0 K+. The first particle is neutral, the second is charged
    static int make_dstoh0hp(
        std::vector<Particle> &dsl,  // to be filled
        std::vector<Particle> &h0l, std::vector<Particle> &hpl,
        std::vector<Particle> &hml, int mode = 0);
    /// Ds+ -> phi pi+, Ks0 K+, separated flavors
    static int make_dstoh0hp(
        std::vector<Particle> &dspl, std::vector<Particle> &dsml,  // to be filled
        std::vector<Particle> &h0l,  std::vector<Particle> &hpl,
        std::vector<Particle> &hml, int mode = 0);
    /// Ds+ -> anti-K*0 K+
    static int make_dstoKK(
        std::vector<Particle> &dsl,  // to be filled
        std::vector<Particle> &kst0bl, std::vector<Particle> &kpl,  // Ds+ -> anti-K*0 K+
        std::vector<Particle> &kst0l,  std::vector<Particle> &kml,  // Ds- -> K*0 K-
        int mode = 1);
    /// Ds+ -> anti-K*0 K+, separated flavors
    static int make_dstoKK(
        std::vector<Particle> &dspl,   std::vector<Particle> &dsml, // to be filled
        std::vector<Particle> &kst0bl, std::vector<Particle> &kpl,  // Ds+ -> anti-K*0 K+
        std::vector<Particle> &kst0l,  std::vector<Particle> &kml,  // Ds- -> K*0 K-
        int mode = 1);
    /// Ds* -> Ds gamma
    static int make_dsstar(PLM& plm, bool separate);
    /// D*s -> Ds gamma
    static int make_dsstar(
        std::vector<Particle> &dsstl,  // to be filled
        std::vector<Particle> &dsl, std::vector<Particle> &gammal);

// * Dsj makers * //
    /// D*sj -> Ds gamma
    static int make_dsjtodsgamma(PLM& plm, bool separate, int mode = 0);
    /// D*sj -> Ds pi0
    static int make_dsjtodspi0(PLM& plm, bool separate, int mode = 1);
    /// D*sj -> Ds pi+ pi-
    static int make_dsjtodspipi(PLM& plm, bool separate, int mode = 2);
    /// D*sj -> Ds pi0 pi0
    static int make_dsjtods2pi0(PLM& plm, bool separate, int mode = 3);

    /// D*sj -> Ds* gamma
    static int make_dsjtodsstgamma(PLM& plm, bool separate, int mode = 10);
    /// D*sj -> Ds* pi0
    static int make_dsjtodsstpi0(PLM& plm, bool separate, int mode = 11);
    /// D*sj -> Ds* pi+ pi-
    static int make_dsjtodsstpipi(PLM& plm, bool separate, int mode = 12);
    /// D*sj -> Ds* pi0 pi0
    static int make_dsjtodsst2pi0(PLM& plm, bool separate, int mode = 13);

    /// D*sj -> Ds gamma gamma
    static int make_dsjtods2gamma(PLM& plm, bool separate, int mode = 10);
    /// D*sj -> Ds pi0 gamma
    static int make_dsjtodspi0gamma(PLM& plm, bool separate, int mode = 11);
    /// D*sj -> Ds pi+ pi- gamma
    static int make_dsjtodspipigamma(PLM& plm, bool separate, int mode = 12);
    /// D*sj -> Ds pi0 pi0 gamma
    static int make_dsjtods2pi0gamma(PLM& plm, bool separate, int mode = 13);

    /// D*sj -> D(*)s X
    static int make_dsjtodsx(
        std::vector<Particle> &dsjl, std::vector<Particle> &dspl,
        std::vector<Particle> &dsml, std::vector<Particle> &xl,
        int mode = 0);
    /// D*sj -> D(*)s X, separated flavors
    static int make_dsjtodsx(
        std::vector<Particle> &dsjpl, std::vector<Particle> &dsjml,
        std::vector<Particle> &dspl, std::vector<Particle> &dsml,
        std::vector<Particle> &xl, int mode = 0);
    /// D*sj -> D(*)s X1 X2
    static int make_dsjtodsxy(
        std::vector<Particle> &dsjl, std::vector<Particle> &dspl,
        std::vector<Particle> &dsml, std::vector<Particle> &xl,
        std::vector<Particle> &yl, int mode = 0);
    /// D*sj -> D(*)s X1 X2, separated flavors
    static int make_dsjtodsxy(
        std::vector<Particle> &dsjpl, std::vector<Particle> &dsjml,
        std::vector<Particle> &dspl, std::vector<Particle> &dsml,
        std::vector<Particle> &xl, std::vector<Particle> &yl,
        int mode = 0);
    /// D*sj -> D(*)s X1 X2 X3
    static int make_dsjtodsxyz(
        std::vector<Particle> &dsjl, std::vector<Particle> &dspl,
        std::vector<Particle> &dsml, std::vector<Particle> &xl,
        std::vector<Particle> &yl,   std::vector<Particle> &zl, int mode = 0);
    /// D*sj -> D(*)s X1 X2 X3, separated flavors
    static int make_dsjtodsxyz(
        std::vector<Particle> &dsjpl, std::vector<Particle> &dsjml,
        std::vector<Particle> &dspl,  std::vector<Particle> &dsml,
        std::vector<Particle> &xl,    std::vector<Particle> &yl,
        std::vector<Particle> &zl, int mode = 0);
    /// D** -> D pi+ pi-
    static int make_dststtoxyz(
        std::vector<Particle>& dststl,
        std::vector<Particle>& xl,
        std::vector<Particle>& yl,
        std::vector<Particle>& zl, int mode);

// * Interface for configuration * //
    /// Set parameter
    static void setPar(const std::string& key, double val);
    /// Set parameter
    static void setSymCut(const std::string& key, double mean, double gate);
    /// Set parameter
    static void setASymCut(const std::string& key, double lo, double hi);

 private:
    static void setH0toGGErrors(std::vector<Particle>& etal);
    static void D0UIS(std::vector<Particle> &v, int mode);
    /// Check if particle list is in PLM. If not, initialize an empty list
    static void checkPLM(PLM& plm, const std::string& pcl);
    static void checkPLM(PLM& plm, const std::string& pcl1, const std::string& pcl2);
};

#if defined(BELLE_NAMESPACE)
}
#endif

#endif  // __COMBINATOR_H__
