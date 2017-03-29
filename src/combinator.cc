#include "combinator.h"

#include "uisetter.h"
#include "recotools.h"
#include "kinefitter.h"

#include "particle/PID.h"
#include "particle/utility.h"
#include "particle/combination.h"
#include "particle/ParticleUserInfo.h"
#include "mdst/findKs.h"
#include "nisKsFinder/nisKsFinder.h"
#include "mdst/mdst.h"
#include "ip/IpProfile.h"

#include MDST_H
#include HEPEVT_H

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif

typedef std::vector<Particle> pvec;
typedef Combinator CMB;
typedef UISetter UIS;

using std::fabs;
using std::abs;
using std::cerr;
using std::endl;

bool   CMB::m_tupfl = false;
bool   CMB::m_mc_flag     = false;
double CMB::m_d0mmin      = 1.86484  - 0.065;
double CMB::m_d0mmax      = 1.86484  + 0.065;
double CMB::m_d0mmin_narrow = 1.86484- 0.035;
double CMB::m_d0mmax_narrow = 1.86484+ 0.035;
double CMB::m_dpmmin      = 1.86961  - 0.065;
double CMB::m_dpmmax      = 1.86961  + 0.065;
double CMB::m_mds_min     = 1.967    - 0.065;
double CMB::m_mds_max     = 1.967    + 0.065;
double CMB::m_mphi_min    = 1.020    - 0.020;
double CMB::m_mphi_max    = 1.020    + 0.020;
double CMB::m_omega_min   = 0.78265  - 0.030;
double CMB::m_omega_max   = 0.78265  + 0.030;
double CMB::m_eta_min_gg  = 0.547862 - 0.035;
double CMB::m_eta_max_gg  = 0.547862 + 0.035;
double CMB::m_eta_min_ppp = 0.547862 - 0.020;
double CMB::m_eta_max_ppp = 0.547862 + 0.020;
double CMB::m_egamma_min  = 0.08;
double CMB::m_egamma_min_pi0 = 0.04;
double CMB::m_egamma_min_eta = 0.08;
double CMB::m_kst0mmin    = 0.89166  - 0.050;
double CMB::m_kst0mmax    = 0.89166  + 0.050;
double CMB::m_dsst_dm_min = 0.1438   - 0.007;
double CMB::m_dsst_dm_max = 0.1438   + 0.007;
double CMB::m_dst0_dm_min = 0.14212  - 0.007;
double CMB::m_dst0_dm_max = 0.14212  + 0.007;
double CMB::m_dstp_dm_min = 0.1454257- 0.007;
double CMB::m_dstp_dm_max = 0.1454257+ 0.007;
double CMB::m_mdsstj_min  = 2.15;
double CMB::m_mdsstj_max  = 2.75;
double CMB::m_mdstst_min  = 2.;
double CMB::m_mdstst_max  = 3.;

void CMB::D0MassCut(std::vector<Particle> &v, const bool narrow) {
    if (narrow) withMassCut(v, m_d0mmin_narrow, m_d0mmax_narrow);
    else        withMassCut(v, m_d0mmin, m_d0mmax);
}

void CMB::D0UIS(std::vector<Particle> &v, const int mode) {
    UIS::SetDlInfo(v);
    UIS::SetModeD(v, mode);
}

int CMB::make_kpi(pvec &pipl, pvec &piml, pvec &kpl, pvec &kml) {
    pipl.clear(); piml.clear(); kpl.clear(); kml.clear();
    const Gen_hepevt &null = Gen_hepevt_Manager::get_manager().get_NULL();

    Mdst_charged_Manager &chg_mgr = Mdst_charged_Manager::get_manager();
    for (std::vector<Mdst_charged>::const_iterator i = chg_mgr.begin();
         i != chg_mgr.end(); i++) {
        const Mdst_charged& track = *i;
        Particle pi(track, Ptype(track.charge() > 0 ? "PI+" : "PI-"));
        Particle K(track, Ptype(track.charge( ) > 0 ? "K+"  : "K-" ));

        if (m_tupfl) {
            pi.userInfo(TrkUserInfo());
            K.userInfo(TrkUserInfo());
            UIS::SetTrkInfo(pi);
            UIS::SetTrkInfo(K);

            const int flag = UIS::CheckTrk(K, true);
            if (flag > 0 && flag != 6) continue;
            const double rip = fabs(dynamic_cast<TrkUserInfo&>(K.userInfo()).r());
            const Gen_hepevt &h = m_mc_flag ? get_hepevt(track) : null;
            if (h) {
                pi.relation().genHepevt(h);
                K.relation().genHepevt(h);
            }
            if( track.charge() > 0) {
            pipl.push_back(pi);
            if (flag != 6) kpl.push_back(K);
            } else {
                piml.push_back(pi);
                if (flag != 6) kml.push_back(K);
            }
        } else { // skim
            const int flag = UIS::CheckTrk(K, false);
            if (flag > 0 && flag != 6) continue;
            if (track.charge() > 0) {
                pipl.push_back(pi);
                if (flag != 6) kpl.push_back(K);
            } else {
                piml.push_back(pi);
                if (flag != 6) kml.push_back(K);
            }
       }
    }
    return pipl.size() + piml.size();
}

int CMB::make_ks(pvec &ksl) {
    ksl.clear();
    const Gen_hepevt &null = Gen_hepevt_Manager::get_manager().get_NULL();
    nisKsFinder ksnb;

    Mdst_vee2_Manager &vee2_mgr = Mdst_vee2_Manager::get_manager();
    const HepPoint3D IP = IpProfile::usable() ? IpProfile::e_position() :
                                                HepPoint3D(0);
    for (std::vector<Mdst_vee2>::iterator i = vee2_mgr.begin();
         i != vee2_mgr.end(); i++) {
        Mdst_vee2 &vee2 = *i;
        int kind = vee2.kind();
        if (kind != 1) continue;
        ksnb.candidates(vee2, IP);
        if (!ksnb.standard()) continue;
        Particle p(vee2);
        if (p.p().m() > 0.52 || p.p().m() < 0.48) continue;
        const Gen_hepevt &h = m_mc_flag ? get_hepevt(vee2) : null;
        if (h) p.relation().genHepevt(h);
        if (m_tupfl) {
            p.userInfo(KsUserInfo());
            dynamic_cast<KsUserInfo&>(p.userInfo()).dr(ksnb.drp()>ksnb.drn() ?
                                                      ksnb.drp() : ksnb.drn());
            dynamic_cast<KsUserInfo&>(p.userInfo()).dz(ksnb.zdist());
            dynamic_cast<KsUserInfo&>(p.userInfo()).dphi(ksnb.dphi());
            dynamic_cast<KsUserInfo&>(p.userInfo()).fl(ksnb.fl());
        }
        ksl.push_back(p);
    }
    return ksl.size();
}

int CMB::make_d0tok0shh(pvec &d0l, pvec &ksl, pvec &hpl, pvec &hml,
                        const int mode) {
    d0l.clear();
    combination(d0l, Ptype("D0"), ksl, hpl, hml);
    D0MassCut(d0l, false);
    if (m_tupfl) D0UIS(d0l, mode);
    return d0l.size();
}

int CMB::make_d0tohh(pvec &d0l, pvec &h1l, pvec &h2l,
                     const int mode, const bool narrow) {
    d0l.clear();
    combination(d0l, Ptype("D0"), h1l, h2l);
    D0MassCut(d0l, narrow);
    if (m_tupfl) D0UIS(d0l, mode);
    return d0l.size();
}

int CMB::make_d0tokpi(pvec &d0l, pvec &kml, pvec &pipl, pvec &kpl, pvec &piml,
                      const int mode) {
    pvec d0bl;
    make_d0tokpi(d0l, d0bl, kml, pipl, kpl, piml, mode);
    d0l.insert(d0l.end(), d0bl.begin(), d0bl.end());
    return d0l.size();
}

int CMB::make_d0tokpi(pvec &d0l, pvec &d0bl, pvec &kml, pvec &pipl,
                      pvec &kpl, pvec &piml, const int mode) {
    d0l.clear(); d0bl.clear();
    combination(d0l, Ptype("D0"), kml, pipl);
    combination(d0bl, Ptype("D0B"), kpl, piml);
    D0MassCut(d0l, false);
    D0MassCut(d0bl, false);
    if(m_tupfl){
        D0UIS(d0l, mode);
        D0UIS(d0bl, mode);
    }
    return d0l.size() + d0bl.size();
}

int CMB::make_kstar0(pvec &kst0l, pvec &kpl, pvec &piml,
                     pvec &kml, pvec &pipl) {
    pvec kst0bl;
    make_kstar0(kst0l, kst0bl, kpl, piml, kml, pipl);
    kst0l.insert(kst0l.end(), kst0bl.begin(), kst0bl.end());
    return kst0l.size();
}

int CMB::make_kstar0(pvec &kst0l, pvec &kst0bl, pvec &kpl, pvec &piml,
                     pvec &kml, pvec &pipl) {
    kst0l.clear(); kst0bl.clear();
    combination(kst0bl, Ptype(-313), kml, pipl);
    withMassCut(kst0bl, m_kst0mmin, m_kst0mmax);
    combination(kst0l, Ptype(313), kpl, piml);
    withMassCut(kst0l, m_kst0mmin, m_kst0mmax);
    if (m_tupfl) {
        UIS::SetKst0lInfo(kst0l);
        UIS::SetKst0lInfo(kst0bl);
    }
    return kst0l.size() + kst0bl.size();
}

int CMB::make_d0tokpipi0(pvec &d0l, pvec &kml, pvec &pipl, pvec &kpl,
                         pvec &piml, pvec &pi0l,
                         const int mode, const bool narrow) {
    pvec d0bl;
    make_d0tokpipi0(d0l, d0bl, kml, pipl, kpl, piml, pi0l, mode, narrow);
    d0l.insert(d0l.end(), d0bl.begin(), d0bl.end());
    return d0l.size();
}

int CMB::make_d0tokpipi0(pvec &d0l, pvec &d0bl, pvec &kml, pvec &pipl,
                         pvec &kpl, pvec &piml, pvec &pi0l,
                         const int mode, const bool narrow) {
    d0l.clear(); d0bl.clear();
    if (!pi0l.size()) return 0;
    combination(d0l, Ptype("D0"), kml, pipl, pi0l);
    combination(d0bl, Ptype("D0B"), kpl, piml, pi0l);
    D0MassCut(d0l, narrow);
    D0MassCut(d0bl, narrow);
    if (m_tupfl) {
        D0UIS(d0l, mode);
        D0UIS(d0bl, mode);
    }
    return d0l.size() + d0bl.size();
}

int CMB::make_d0tokpipipi(pvec &d0l, pvec &kml, pvec &pipl, pvec &kpl,
                          pvec &piml, const int mode, const bool narrow) {
    pvec d0bl;
    make_d0tokpipipi(d0l, d0bl, kml, pipl, kpl, piml, mode, narrow);
    d0l.insert(d0l.end(), d0bl.begin(), d0bl.end());
    return d0l.size();
}

int CMB::make_d0tokpipipi(pvec &d0l, pvec &d0bl, pvec &kml, pvec &pipl,
                          pvec &kpl, pvec &piml,
                          const int mode, const bool narrow) {
    d0l.clear(); d0bl.clear();
    combination(d0l,Ptype("D0"), pipl, piml, pipl, kml);
    combination(d0bl,Ptype("D0B"), piml, pipl, piml, kpl);
    D0MassCut(d0l, narrow);
    D0MassCut(d0bl, narrow);
    if (m_tupfl) {
        D0UIS(d0l, mode);
        D0UIS(d0bl, mode);
    }
    return d0l.size() + d0bl.size();
}

int CMB::make_dptokpipi(pvec &dpl, pvec &kml, pvec &pipl,
                        pvec &kpl, pvec &piml, const int mode) {
    pvec dml;
    make_dptokpipi(dpl, dml, kml, pipl, kpl, piml, mode);
    dpl.insert(dpl.end(), dml.begin(), dml.end());
    return dpl.size();
}

int CMB::make_dptokpipi(pvec &dpl, pvec &dml, pvec &kml, pvec &pipl,
                        pvec &kpl, pvec &piml, const int mode) {
    dpl.clear(); dml.clear();
    combination(dpl, Ptype("D+"), kml, pipl, pipl);
    withMassCut(dpl, m_dpmmin, m_dpmmax);
    combination(dml, Ptype("D-"), kpl, piml, piml);
    withMassCut(dml, m_dpmmin, m_dpmmax);
    if (m_tupfl) {
        UIS::SetDlInfo(dpl);
        UIS::SetDlInfo(dml);
        UIS::SetModeD(dpl, mode);
        UIS::SetModeD(dml, mode);
    }
    return dpl.size() + dml.size();
}

int CMB::make_pi0(pvec &pi0l) {
    pi0l.clear();
    const Gen_hepevt &null = Gen_hepevt_Manager::get_manager().get_NULL();
// pi0 > gamma gamma
    Mdst_pi0_Manager &pi0_mgr = Mdst_pi0_Manager::get_manager();
    for (std::vector<Mdst_pi0>::const_iterator i = pi0_mgr.begin();
         i != pi0_mgr.end(); i++) {
        const Mdst_pi0& pi0 = *i;
        Particle p(pi0);
        if (p.child(0).p().t() < m_egamma_min_pi0 ||
            p.child(1).p().t() < m_egamma_min_pi0) continue;
        if (m_tupfl) {
            const Gen_hepevt &h = m_mc_flag ? get_hepevt(pi0) : null;
            if (h) p.relation().genHepevt(h);
            const Gen_hepevt &hg1 = m_mc_flag ?
                  get_hepevt(p.child(0).mdstGamma()) : null;
            if (hg1) p.child(0).relation().genHepevt(hg1);
            const Gen_hepevt &hg2 = m_mc_flag ?
                  get_hepevt(p.child(1).mdstGamma()) : null;
            if (hg2) p.child(1).relation().genHepevt(hg2);
            p.userInfo(Pi0UserInfo());
            dynamic_cast<Pi0UserInfo&>(p.userInfo()).Mass(pi0.mass());
            dynamic_cast<Pi0UserInfo&>(p.userInfo()).Chi2(pi0.chisq());
            setGammasError(p,HepPoint3D(0),1.*HepSymMatrix(3,1));  // Necessary for kinematic fitting
        }
        pi0l.push_back(p);
    }
    return pi0l.size();
}

void CMB::setH0toGGErrors(pvec& etal) {
    for (unsigned i = 0; i < etal.size(); i++)
        setGammasError(etal[i], HepPoint3D(0), 1.*HepSymMatrix(3, 1));
}

int CMB::make_gamma(pvec& gl) {
    gl.clear(); makeGamma(gl); withPCut(gl, m_egamma_min);
    return gl.size();
}

int CMB::make_etato2g(pvec& etal, pvec& gammas, const int mode) {
    etal.clear();
// eta -> gamma gamma
    gammas.clear();
    makeGamma(gammas); withPCut(gammas, m_egamma_min_eta);
    combination(etal, Ptype(221), gammas, gammas);
    withMassCut(etal, m_eta_min_gg, m_eta_max_gg);
    if (m_tupfl) {
        UIS::Seth0lInfo(etal);
        UIS::SetModeEta(etal, mode);
        CMB::setH0toGGErrors(etal);
    }
    return etal.size();
}

int CMB::make_etato3pi(pvec& etal, pvec& pipl, pvec& piml, pvec& pi0l,
                       const int mode) {
    etal.clear();
    combination(etal, Ptype(221), pipl, piml, pi0l);
    withMassCut(etal, m_eta_min_ppp, m_eta_max_ppp);
    if (m_tupfl) {
        UIS::Seth0lInfo(etal);
        UIS::SetModeEta(etal, mode);
    }
    return etal.size();
}

int CMB::make_omega(pvec& omegal, pvec& pipl, pvec& piml, pvec& pi0l) {
    omegal.clear();
// omega -> pi+ pi- pi0
    combination(omegal, Ptype(223), pipl, piml, pi0l);
    withMassCut(omegal, m_omega_min, m_omega_max);
    if (m_tupfl) UIS::Seth0lInfo(omegal);
    return omegal.size();
}

int CMB::make_phitokk(pvec &phil, pvec &kpl, pvec &kml) {
    phil.clear();
    combination(phil, Ptype("PHI"), kpl, kml);
    withMassCut(phil, m_mphi_min, m_mphi_max);
    if (m_tupfl) UIS::SetPhilInfo(phil);
    return phil.size();
}

int CMB::make_dstoh0hp(pvec &dsl, pvec &h0l, pvec &hpl, pvec &hml,
                       const int mode) {
    pvec dsml;
    make_dstoh0hp(dsl, dsml, h0l, hpl, hml, mode);
    dsl.insert(dsl.end(), dsml.begin(), dsml.end());
    return dsl.size();
}

int CMB::make_dstoh0hp(pvec &dspl, pvec &dsml, pvec &h0l,
                       pvec &hpl, pvec &hml, const int mode) {
    dspl.clear(); dsml.clear();
    combination(dspl, Ptype( 431), h0l, hpl);
    combination(dsml, Ptype(-431), h0l, hml);
    withMassCut(dspl, m_mds_min, m_mds_max);
    withMassCut(dsml, m_mds_min, m_mds_max);
    if (m_tupfl) {
        UIS::SetDlInfo(dspl);
        UIS::SetDlInfo(dsml);
        UIS::SetModeD(dspl, mode);
        UIS::SetModeD(dsml, mode);
    }
    return dspl.size() + dsml.size();
}

int CMB::make_dstoKK(pvec &dsl, pvec &kst0bl, pvec &kpl,
                     pvec &kst0l, pvec &kml, const int mode) {
    pvec dsml;
    make_dstoKK(dsl, dsml, kst0bl, kpl, kst0l, kml, mode);
    dsl.insert(dsl.end(), dsml.begin(), dsml.end());
    return dsl.size();
}

int CMB::make_dstoKK(pvec &dspl, pvec &dsml, pvec &kst0bl, pvec &kpl,
                     pvec &kst0l, pvec &kml, const int mode) {
    dspl.clear(); dsml.clear();
    combination(dspl, Ptype(431), kst0bl, kpl);
    combination(dsml, Ptype(-431), kst0l, kml);
    withMassCut(dspl, m_mds_min, m_mds_max);
    withMassCut(dsml, m_mds_min, m_mds_max);
    if (m_tupfl) {
        UIS::SetDlInfo(dspl);
        UIS::SetDlInfo(dsml);
        UIS::SetModeD(dspl, mode);
        UIS::SetModeD(dsml, mode);
    }
    return dspl.size() + dsml.size();
}

int CMB::make_dsstar(pvec &dsstl, pvec &dsl, pvec &gammal) {
    dsstl.clear();
    if (!dsl.size() || !gammal.size()) return 0;
    combination(dsstl, dsl[0].pType().charge() > 0 ? Ptype(433) :
                                                     Ptype(-433), dsl, gammal);
    withMassDifCut(dsstl, m_dsst_dm_min, m_dsst_dm_max, 0);
    return dsstl.size();
}

int CMB::make_dstar(pvec &dstl, pvec &dl, pvec &xl) {
    dstl.clear();
    if (!dl.size() || !xl.size()) return 0;
    int dstlund = 0;
    double dmmin = 0;
    double dmmax = 0;
    if (dl[0].lund() == 431 && xl[0].lund() == 22) {
        dstlund =  433;  // D_s*+ -> Ds+ gamma
        dmmin = m_dsst_dm_min; dmmax = m_dsst_dm_max;
    } else if (dl[0].lund() == -431 && xl[0].lund() == 22) {
        dstlund = -433;  // D_s*- -> Ds- gamma
        dmmin = m_dsst_dm_min; dmmax = m_dsst_dm_max;
    } else if (dl[0].lund() ==  421 && xl[0].lund() == 111) {
        dstlund = 423;  // D*0 -> D0 pi0
        dmmin = m_dst0_dm_min; dmmax = m_dst0_dm_max;
    } else if (dl[0].lund() == -421 && xl[0].lund() == 111) {
        dstlund = -423;  // anti-D*0 -> anti-D0 pi0
        dmmin = m_dst0_dm_min; dmmax = m_dst0_dm_max;
    } else if (abs(dl[0].lund()) == 421 && xl[0].lund() ==  211) {
        dstlund = 413;  //D*+ -> D0 pi+
        dmmin = m_dstp_dm_min; dmmax = m_dstp_dm_max;
    } else if (abs(dl[0].lund()) == 421 && xl[0].lund() == -211) {
        dstlund = -413;  //D*- -> anti-D0 pi-
        dmmin = m_dstp_dm_min; dmmax = m_dstp_dm_max;
    } else {
        cerr << "CMB::make_dstar: wrong particles "
             << dl[0].lund() << " " << xl[0].lund() << endl;
        return 0;
    }
    const Ptype dsttype(dstlund);
    combination(dstl, dsttype, dl, xl);
    withMassDifCut(dstl, dmmin, dmmax, 0);
    return dstl.size();
}

int CMB::make_dsjtodsx(pvec &dsjl, pvec &dspl, pvec &dsml, pvec &xl,
                       const int mode) {
    pvec dsjml;
    make_dsjtodsx(dsjl, dsjml, dspl, dsml, xl, mode);
    dsjl.insert(dsjl.end(), dsjml.begin(), dsjml.end());
    return dsjl.size();
}

int CMB::make_dsjtodsx(pvec &dsjpl, pvec &dsjml, pvec &dspl,
                       pvec &dsml, pvec &xl, const int mode) {
    dsjpl.clear(); dsjml.clear();
    if (!xl.size()) return 0;
    if (dspl.size()) {
        combination(dsjpl, Ptype(431), dspl, xl);
        withMassCut(dsjpl, m_mdsstj_min, m_mdsstj_max);
    }
    if (dsml.size()) {
        combination(dsjml, Ptype(-431), dsml, xl);
        withMassCut(dsjml, m_mdsstj_min, m_mdsstj_max);
    }
    if (m_tupfl) {
        UIS::SetDlInfo(dsjpl);
        UIS::SetDlInfo(dsjml);
        UIS::SetModeD(dsjpl, mode);
        UIS::SetModeD(dsjml, mode);
    }
    return dsjpl.size() + dsjml.size();
}

int CMB::make_dsjtodsxy(pvec &dsjl, pvec &dspl, pvec &dsml,
                        pvec &xl, pvec &yl, const int mode) {
    pvec dsjml;
    make_dsjtodsxy(dsjl, dsjml, dspl, dsml, xl, yl, mode);
    dsjl.insert(dsjl.end(), dsjml.begin(), dsjml.end());
    return dsjl.size();
}

int CMB::make_dsjtodsxy(pvec &dsjpl, pvec &dsjml, pvec &dspl,
                        pvec &dsml, pvec &xl, pvec &yl, const int mode) {
    dsjpl.clear(); dsjml.clear();
    if (!xl.size() || !yl.size()) return 0;
    if (dspl.size()) {
        combination(dsjpl, Ptype(431), dspl, xl, yl);
        withMassCut(dsjpl, m_mdsstj_min, m_mdsstj_max);
    }
    if (dsml.size()) {
        combination(dsjml, Ptype(-431), dsml, xl, yl);
        withMassCut(dsjml, m_mdsstj_min, m_mdsstj_max);
    }
    if (m_tupfl) {
        UIS::SetDlInfo(dsjpl);
        UIS::SetDlInfo(dsjml);
        UIS::SetModeD(dsjpl, mode);
        UIS::SetModeD(dsjml, mode);
    }
    return dsjpl.size() + dsjml.size();
}

int CMB::make_b0toxy(pvec& b0l, pvec& xpl, pvec& yml,
                     pvec& xml, pvec& ypl, const int mode) {
    b0l.clear();
    const bool b0flag  = xpl.size() && yml.size();
    const bool b0bflag = xml.size() && ypl.size();
    if (!b0flag && !b0bflag) return 0;
    if (b0flag) combination(b0l, Ptype("B0"), xpl, yml);
    if (b0bflag) {
        pvec b0bl;
        combination(b0bl,Ptype(-511), xml, ypl);
        b0l.insert(b0l.end(), b0bl.begin(), b0bl.end());
    }
    double mbc, de;
    for (unsigned i = 0; i < b0l.size(); i++) {
        Particle& B0 = b0l[i];
        RTools::Mbc_deltaE(B0,mbc,de);
        if (!m_tupfl) {  // skim
            if (fabs(de) < 0.3 && mbc > 5.22 && mbc < 5.30) {
                if (!RTools::IsDuplicated(B0)) return 1;
            }
        } else {  // ntuple
            if (de > 0.3 || de < -0.3 || mbc < 5.22 || mbc > 5.30) {
                b0l.erase(b0l.begin()+i); i--; continue;
            }
            if (RTools::IsDuplicated(B0)) {
                b0l.erase(b0l.begin()+i); i--; continue;
            }
            B0.userInfo(BUserInfo());
            UIS::SetMbc_deltaE(B0,mbc,de);
        }
    }
    if (m_tupfl) {
        UIS::SetModeB(b0l, mode);
        return b0l.size();
    }
    return 0;
}

int CMB::make_bptoxy(pvec& bpl, pvec& x0l, pvec& ypl, const int mode) {
    bpl.clear();
    if (!x0l.size() || !ypl.size()) return 0;
    combination(bpl, ypl[0].pType().charge() > 0 ? Ptype("B+") : Ptype("B-"), x0l, ypl);
    double mbc, de;
    for (unsigned i = 0; i < bpl.size(); i++) {
        Particle& Bp = bpl[i];
        RTools::Mbc_deltaE(Bp,mbc,de);
        if (!m_tupfl) {  // skim
            if (fabs(de) < 0.3 && mbc > 5.22 && mbc < 5.30) {
                if(!RTools::IsDuplicated(Bp)) return 1;
            }
        } else {
            if (de > 0.3 || de < -0.3 || mbc < 5.22 || mbc > 5.30) {
                bpl.erase(bpl.begin() + i); i--; continue;
            }
            if (RTools::IsDuplicated(Bp)){
                bpl.erase(bpl.begin() + i); i--; continue;
            }
            Bp.userInfo(BUserInfo());
            UIS::SetMbc_deltaE(Bp, mbc, de);
        }
    }
    if (m_tupfl) {
        UIS::SetModeB(bpl, mode);
        return bpl.size();
    }
    return 0;
}

int CMB::make_b0tod0pipi(pvec& b0l, pvec &d0l, pvec &pipl, pvec &piml) {
    b0l.clear();
    combination(b0l,Ptype("B0"),d0l,pipl,piml);
    double mbc, de;
    for (unsigned i = 0; i < b0l.size(); i++) {
        Particle& B0 = b0l[i];
        RTools::Mbc_deltaE(B0,mbc,de);
        if (!m_tupfl) {
            if (fabs(de) < 0.35 && mbc > 5.18 && mbc < 5.31) {
                if (!RTools::IsDuplicated(B0)) return 1;
            }
        } else {  // tuple
            B0.userInfo(B0UserInfo());
            const Particle& D0 = B0.child(0);
            if (de > 0.3 || de < -0.3 || mbc < 5.25 || mbc > 5.30) {
                b0l.erase(b0l.begin()+i); i--; continue;
            }
            if (RTools::IsDuplicated(B0)) {
                b0l.erase(b0l.begin()+i); i--; continue;
            }
            UIS::SetB0toDhhRawInfo(B0);

            if (make_vtx_for_b0tod0pipi(B0)) {
                UIS::SetBadVtx(B0);
                UIS::SetMbc_deltaE(B0, mbc, de);
                continue;
             }
             if (make_mass_for_b0tod0pipi(B0)) {
                 UIS::SetBadMass(B0);
             }      
             UIS::SetMbc_deltaE(B0,mbc,de);
         }
    }
    return m_tupfl ? b0l.size() : 0;
}

int CMB::make_vtx_for_b0tod0pipi(Particle& b0) {
    const int d0index = RTools::ChldIndex(b0, 421, true);
    if (d0index < 0) return -4;  // There is no D0 among children
    Particle& D0 = b0.child(d0index);
    // * Vertex fitting * //
    DUserInfo& dinfo = dynamic_cast<DUserInfo&>(D0.userInfo());
    if (!dinfo.IsVtxFitDone()) {
        double d0_vtx_chisq = -1;
        const int d0_vtx_fit_flag = KineFitter::vtx_fit_r(D0, d0_vtx_chisq);
        dinfo.VtxChi2(d0_vtx_chisq);
        dinfo.VtxFitDone();
        dinfo.VtxFitGood(!d0_vtx_fit_flag);
    }
    if (!dinfo.IsVtxFitGood()) {
        cerr << "Bad D0 vtx fit for mode " << dinfo.Mode() << endl;
        return -1;
    }
    int fit_flag = 0; 
    kvertexfitter kvf = KineFitter::vtx_fit(b0, true, fit_flag);
    if (fit_flag) return -2;
    UIS::SetSigVtxFitInfo(b0, kvf);
    if (RTools::TagVertex(b0)) return -3;
    return 0;
}

int CMB::make_mass_for_b0tod0pipi(Particle& b0) {
    const int d0index = RTools::ChldIndex(b0, 421, true);
    if (d0index < 0) return -4;  // There is no D0 among children
    Particle& D0 = b0.child(d0index);
    DUserInfo& dinfo = dynamic_cast<DUserInfo&>(D0.userInfo());
    if (!dinfo.IsMassFitDone()) {
        double d0_mass_chisq = -1;
        const int d0_mass_fit_flag = KineFitter::mass_fit_r(D0, d0_mass_chisq);
        dinfo.MassChi2(d0_mass_chisq);
        dinfo.MassFitDone();
        dinfo.MassFitGood(!d0_mass_fit_flag);
    }
    if(!dinfo.IsMassFitGood()){
        cerr << "Bad D0 mass fit for mode " << dinfo.Mode() << endl;
        return -1;
    }
    kmassfitter kmf = KineFitter::mass_fit(b0, true);
    UIS::SetB0toDhhMassFitInfo(b0, kmf);
    return 0;
}

int CMB::make_b0tod0pi0pi0(pvec& b0l, pvec &d0l, pvec &pi0l) {
    b0l.clear();
    combination(b0l, Ptype("B0"), d0l, pi0l, pi0l);
    double mbc, de;
    for (unsigned i = 0; i < b0l.size(); i++) {
        Particle& B0 = b0l[i];
        RTools::Mbc_deltaE(B0,mbc,de);
        if (!m_tupfl) {
            if (fabs(de) < 0.32 && mbc > 5.18 && mbc < 5.31) {
                if (!RTools::IsDuplicated(B0)) return 1;
            }
        } else {
            B0.userInfo(B0UserInfo());
            if (de > 0.3 || de < -0.3 || mbc < 5.2 || mbc > 5.3) {
                b0l.erase(b0l.begin()+i); i--;
                continue;
            }
            if (RTools::IsDuplicated(B0)) {
                b0l.erase(b0l.begin()+i); i--;
                continue;
            }
            int d0index = 0;
            if(abs(B0.child(0).lund()) == 421) d0index = 0;
            else if(abs(B0.child(1).lund()) == 421) d0index = 1;
            else if(abs(B0.child(2).lund()) == 421) d0index = 2;
            else continue;
            Particle& D0 = B0.child(d0index);
            DUserInfo& dinfo = dynamic_cast<DUserInfo&>(D0.userInfo());
            if (!dinfo.IsVtxFitDone()) {
                double d0_vtx_chisq = -1;
                const int d0_vtx_fit_flag = KineFitter::vtx_fit_r(D0,d0_vtx_chisq);
                dinfo.VtxChi2(d0_vtx_chisq);
                dinfo.VtxFitDone();
                dinfo.VtxFitGood(!d0_vtx_fit_flag);
            }
            const bool d0_vtx_fit_good = ((const DUserInfo&)D0.userInfo()).IsVtxFitGood();
            if (!d0_vtx_fit_good) {
                b0l.erase(b0l.begin()+i); i--;
                continue;
            }
            if (KineFitter::b0tod0pipi_d0vtx_fit(B0)) {
                b0l.erase(b0l.begin()+i); i--;
                continue;
            }
            if (RTools::TagVertex(B0)) {
                b0l.erase(b0l.begin()+i); i--;
                continue;
            }
            UIS::SetMbc_deltaE(B0,mbc,de);
        }
     }
     return m_tupfl ? b0l.size() : 0;
}

int CMB::make_dststtoxyz(pvec& dststl, pvec& xl, pvec& yl, pvec& zl,
                         const int mode) {
    dststl.clear();
    if (!xl.size()) return 0;
    if (!yl.size()) return 0;
    if (!zl.size()) return 0;
    combination(dststl, Ptype(10411), xl, yl, zl);
    withMassCut(dststl, m_mdstst_min, m_mdstst_max);
    if (m_tupfl) {
        UIS::SetDlInfo(dststl);
        UIS::SetModeD(dststl, mode);
    }
    return dststl.size();
}

#if defined(BELLE_NAMESPACE)
}
#endif

