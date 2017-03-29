binfo.cc                                                                                            0000644 0047266 0051066 00000001077 12770400276 012173  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "binfo.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///////////////
//     B     //
///////////////
BUserInfo::BUserInfo() :
  m_de(0.),
  m_mbc(0.),
  m_mode(-1)
{
}


BUserInfo::~BUserInfo()
{
}

BUserInfo::BUserInfo(const BUserInfo &x)
{
  *this = x;
}

BUserInfo* BUserInfo::clone(void) const
{
  BUserInfo *x = new BUserInfo( *this );
  return x;
}

BUserInfo & BUserInfo::operator = (const BUserInfo &x)
{
  m_de = x.m_de;
  m_mbc = x.m_mbc;
  m_mode = x.m_mode;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 


                                                                                                                                                                                                                                                                                                                                                                                                                                                                 combinator.cc                                                                                       0000644 0047266 0051066 00000062303 13066614626 013237  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "combinator.h"

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

                                                                                                                                                                                                                                                                                                                             d0info.cc                                                                                           0000644 0047266 0051066 00000001303 12770400276 012245  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "d0info.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

////////////////
//     D0     //
////////////////
D0UserInfo::D0UserInfo() :
  DUserInfo(),
  m_m2p(0),
  m_m2m(0),
  m_m2p_raw(0),
  m_m2m_raw(0)
{
}

D0UserInfo::~D0UserInfo()
{
}

D0UserInfo::D0UserInfo(const D0UserInfo &x) :
  DUserInfo(x)
{
  *this = x;
}

D0UserInfo* D0UserInfo::clone(void) const
{
  D0UserInfo *x = new D0UserInfo(*this);
  return x;
}

D0UserInfo & D0UserInfo::operator = (const D0UserInfo &x)
{
  DUserInfo::operator=(x);
  m_m2p     = x.m_m2p;
  m_m2m     = x.m_m2m;
  m_m2p_raw = x.m_m2p_raw;
  m_m2m_raw = x.m_m2m_raw;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
                                                                                                                                                                                                                                                                                                                             DalitzVars.cc                                                                                       0000644 0047266 0051066 00000000224 12770400276 013152  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "DalitzVars.h"

ClassImp(DalitzVars)

void DalitzVars::Clear(void){
  for(int i=0; i<3; i++){
    msq[i] = -99.;
    cosh[i] = -99.;
  }
}
                                                                                                                                                                                                                                                                                                                                                                            DeltaEMbc.cc                                                                                        0000644 0047266 0051066 00000000146 12770400276 012652  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "DeltaEMbc.h"

ClassImp(DeltaEMbc)

void DeltaEMbc::Clear(void){
  de = -99;
  mbc = -99;
}

                                                                                                                                                                                                                                                                                                                                                                                                                          dinfo.cc                                                                                            0000644 0047266 0051066 00000001557 12770400276 012200  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "dinfo.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

////////////////////
// D0, D+ and Ds+ //
////////////////////
DUserInfo::DUserInfo() :
  m_mass(0.),
  m_vfit_done(false),
  m_vfit_good(false),
  m_vtx_chi2(-1.),
  m_mfit_done(false),
  m_mfit_good(false),
  m_mass_chi2(-1.),
  m_mode(-1)
{
}

DUserInfo::~DUserInfo()
{
}

DUserInfo::DUserInfo(const DUserInfo &x)
{
  *this = x;
}

DUserInfo* DUserInfo::clone(void) const
{
  DUserInfo *x = new DUserInfo( *this );
  return x;
}

DUserInfo& DUserInfo::operator=(const DUserInfo &x){
  m_mass      = x.m_mass;
  m_vfit_done = x.m_vfit_done;
  m_vfit_good = x.m_vfit_good;
  m_vtx_chi2  = x.m_vtx_chi2;
  m_mfit_done = x.m_mfit_done;
  m_mfit_good = x.m_mfit_good;
  m_mass_chi2 = x.m_mass_chi2;
  m_mode      = x.m_mode;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
                                                                                                                                                 etainfo.cc                                                                                          0000644 0047266 0051066 00000001233 12770400276 012515  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "etainfo.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///////////////
//    eta    //
///////////////
EtaUserInfo::EtaUserInfo() :
  m_mass(0),
  m_chi2(-1),
  m_mode(-1),
  m_gamma_energy(0)
{
}

EtaUserInfo::~EtaUserInfo()
{
}

EtaUserInfo::EtaUserInfo(const EtaUserInfo &x) 
{
  *this = x;
}

EtaUserInfo* EtaUserInfo::clone(void) const
{
  EtaUserInfo *x = new EtaUserInfo(*this);
  return x;
}

EtaUserInfo & EtaUserInfo::operator = (const EtaUserInfo &x)
{
  m_mass = x.m_mass;
  m_chi2 = x.m_chi2;
  m_gamma_energy = x.m_gamma_energy;
  m_mode = x.m_mode;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
                                                                                                                                                                                                                                                                                                                                                                     EvtInfo.cc                                                                                          0000644 0047266 0051066 00000000245 12770400276 012444  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "EvtInfo.h"

ClassImp(EvtInfo)

void EvtInfo::Clear(void){
  exp = 0;
  run = 0;
  evtn = 0;
}

int EvtInfo::SVD(void) const{
  return exp > 30 ? 2 : 1;
}

                                                                                                                                                                                                                                                                                                                                                           EvtShape.cc                                                                                         0000644 0047266 0051066 00000000231 12770400276 012604  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "EvtShape.h"

ClassImp(EvtShape)

void EvtShape::Clear(void){
  cos_thr = -2.;
  thr_sig = -2.;
  thr_oth = -2.;
  k0.Clear();
  k1.Clear();
}

                                                                                                                                                                                                                                                                                                                                                                       GammaInfo.cc                                                                                        0000644 0047266 0051066 00000000532 12770400276 012727  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "GammaInfo.h"
#include <cmath>

ClassImp(GammaInfo)

void GammaInfo::Clear(void){
  p[0] = -99; p[1] = -99; p[2] = -99;
}

double GammaInfo::P(void) const{
  return sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
}

double GammaInfo::pt(void) const{
  return sqrt(p[0]*p[0]+p[1]*p[1]);
}

double GammaInfo::costh(void) const{
  return p[2]/P();
}

                                                                                                                                                                      GenHepEvt.cc                                                                                        0000644 0047266 0051066 00000000174 12770400276 012720  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "GenHepEvt.h"

ClassImp(GenHepEvt)

void GenHepEvt::Clear(void){
  idhep.clear();
  daF.clear();
  daL.clear();
}

                                                                                                                                                                                                                                                                                                                                                                                                    geninfo.cc                                                                                          0000644 0047266 0051066 00000057017 12770400276 012530  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                // ******************************************************************
// MC Matching
// author: A. Zupanc (anze.zupanc@ijs.si)
//
// Description: TODO
//
// ******************************************************************
#include "belle.h"
#include "panther/panther.h"
#include "mdst/mdst.h"
#include MDST_H
#include HEPEVT_H

#include "particle/utility.h"

#include "geninfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif


using namespace std;

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

int isFSP(Gen_hepevt P) {
	switch (abs(P.idhep())) {
		case 211:
			return 1;
		case 321:
			return 1;
		case 11:
			return 1;
		case 13:
			return 1;
		case 22:
			return 1;
		case 2212:
			return 1;
		case 111:
			return 1;
		case 310:
			return 1;
		case 130:
			return 1;
		case 2112:
			return 1;
		default:
			return 0;
	}
}
void appendRecFSP(Particle p, std::vector<Particle> &children) {
	for (int i = 0; i < (int)p.nChildren(); ++i) {
		if(p.child(i).nChildren() && p.child(i).lund()!=111 && p.child(i).lund()!=310) {
			appendRecFSP(p.child(i),children);
		} else {
			children.push_back(p.child(i));
		}
	}
}

int appendGenFSP(const Gen_hepevt &gen, std::vector<Gen_hepevt> &children) {
	Gen_hepevt_Manager& GenMgr = Gen_hepevt_Manager::get_manager();

	for (int i = gen.daFirst(); i <= gen.daLast(); ++i) {
		if(i==0) {
			std::cout << "[GenInfo] appendGenFSP: requesting Particle with ID = 0! Exiting." << std::endl;
			return 0;
		}

		const Gen_hepevt& child = GenMgr(Panther_ID(i));
		int ndaug2 = (child.daLast()-child.daFirst()) + 1;
		if(ndaug2 && !isFSP(child)) {
			appendGenFSP(child,children);
		} else {
			children.push_back(child);
		}
	}
	return 1;
}

int dumpGenFSP(const Gen_hepevt &gen) {
	Gen_hepevt_Manager& GenMgr = Gen_hepevt_Manager::get_manager();

	int ndaug = (gen.daLast()-gen.daFirst()) + 1;
	std::cout << "Appending FS children for " << gen.idhep() << " with nChildren = " << ndaug << std::endl;
	for (int i = gen.daFirst(); i <= gen.daLast(); ++i) {
		std::cout << "searching for particle with ID = " << i << std::endl;
		if(i==0) {
			std::cout << "[GenInfo] appendGenFSP: requesting Particle with ID = 0! Exiting." << std::endl;
			return 0;
		}

		const Gen_hepevt& child = GenMgr(Panther_ID(i));
		if(!child)
			std::cout << "Particle with this PantherID does not exist." << std::endl;
		int ndaug2 = (child.daLast()-child.daFirst()) + 1;
		std::cout << " -> child " << i << ": ID = " << i << "with nChildren = " << ndaug2 << std::endl;
		if(ndaug2 && !isFSP(child)) {
			std::cout << "not FSP: " << child.idhep() << std::endl;
			dumpGenFSP(child);
		} else {
			std::cout << "Appending FSP: " << child.idhep() << std::endl;
		}
	}
	return 1;
}

int commonMother(std::vector<int> &mothers) {
	if(mothers.size()==0) {
		return 0;
	} else if(mothers.size()==1) {
		return mothers[0];
	}

	int motherID = mothers[0];
	for (int i = 1; i < (int)mothers.size(); ++i) {
		if(motherID!=mothers[i]) {
			return 0;
		}
	}
	return motherID;
}

void fillMothers(Particle &p, std::vector<int> &thisMothers, std::vector<int> &otherMothers) {
	Gen_hepevt motherThis(p.child(0).genHepevt());

	thisMothers.push_back(motherThis.get_ID());

	while(motherThis.mother()) {
		motherThis = motherThis.mother();
		thisMothers.push_back(motherThis.get_ID());
	}

	for(int i = 1; i<(int)p.nChildren(); ++i) {
		motherThis = p.child(i).genHepevt();
		// new
		otherMothers.push_back(motherThis.get_ID());
		while(motherThis.mother()) {
			motherThis = motherThis.mother();
			otherMothers.push_back(motherThis.get_ID());
		}
	}
}

int findCommonMother(int nChildren, std::vector<int> thisMothers, std::vector<int> otherMothers) {
	for (int i = 0; i < (int)thisMothers.size(); ++i) {
		int counter = 0;
		for (int j = 0; j < (int)otherMothers.size(); ++j) {
			if(thisMothers[i]==otherMothers[j])
				counter++;
		}
		if(counter==nChildren-1)
			return thisMothers[i];
	}
	return 0;
}
int findCommonMother(Gen_hepevt pThis, Gen_hepevt pOther, int level) {
	Gen_hepevt motherThis(pThis);

	int i = 1;
	while(motherThis.mother()) {
		motherThis = motherThis.mother();
		i++;

		if(i>level) {
			Gen_hepevt motherOther(pOther);
			while(motherOther.mother()) {
				motherOther = motherOther.mother();
				if(motherThis.get_ID()==motherOther.get_ID())
					return motherThis.get_ID();
			}
		}
	}
	return 0;
}

// TODO
int compareFinalStates(std::vector<Particle> reconstructed, std::vector<Gen_hepevt> generated) {
	if(reconstructed.size() == generated.size()) {
		int missID = 0;
		int missPi0 = 0;
		for (int i = 0; i < (int)reconstructed.size(); ++i) {
			if(reconstructed[i].genHepevt()) {
				int link = 0;
				for (int j = 0; j < (int)generated.size(); ++j) {
					if(reconstructed[i].genHepevt().get_ID()==generated[j].get_ID()) {
						link = 1;
						if(reconstructed[i].lund()!=generated[j].idhep())
							missID++;
						if(reconstructed[i].lund()==111 && getMCtruthPi0Flag(reconstructed[i])!=1) {
							missPi0++;
						}
						break;
					}
				}
				if(!link) {
					std::cout << "[GenInfo] compareFinalStates: Particle (" << reconstructed[i].lund() << " with hepevt = " << reconstructed[i].genHepevt().idhep() << ") not found in list of FSP (gen)!" << std::endl;
					return -11;
				}
			} else {
				std::cout << "[GenInfo] compareFinalStates: Particle without link to genHepevt! [-10]" << std::endl;
				return -10;
			}
		}
		if(missID && missPi0)
			return 6;
		else if(missID)
			return 2;
		else if(missPi0)
			return 5;

		return 1;
	} else if(reconstructed.size() < generated.size()) { // missing particle
		int missing = 0;
		std::vector<int> missP;
		for (int i = 0; i < (int)generated.size(); ++i) {
			int link = 0;
			for (int j = 0; j < (int)reconstructed.size(); ++j) {
				if(reconstructed[j].genHepevt()) {
					if(reconstructed[j].genHepevt().get_ID()==generated[i].get_ID()) {
						link = 1;
						break;
					}
				} else {
					std::cout << "[GenInfo] compareFinalStates: Particle without link to genHepevt! [-9]" << std::endl;
					return -9;
				}
			}
			if(!link) {
				missing++;
				missP.push_back(i);
			}
		}
		if(missing) {
			int masslessOnly = 1;
			int missNu = 0;
			int missGfsr = 0;
			int missGrad = 0;
			for (int i = 0; i < (int)missP.size(); ++i) {
				// new way : distinguish between missing neutrino and FSR gamma and radiative gamma (the latter separation is not 100% precise)
				if(abs(generated[missP[i]].idhep())==12 || abs(generated[missP[i]].idhep())==14 || abs(generated[missP[i]].idhep())==16) {
					// neutrino is missing
					missNu = 1;
				} else if(generated[missP[i]].idhep()==22) {
					// photon is missing (FSR or radiative?)
					if(generated[missP[i]].mother()) {
						int ndaug = (generated[missP[i]].mother().daLast()-generated[missP[i]].mother().daFirst()) + 1;
						if(ndaug==2)
							missGrad = 1;
						else
							missGfsr = 1;
					} else
						missGfsr = 1;
				} else
					masslessOnly = 0;
			}
			if(missNu) {
				if(missGrad && !masslessOnly)
					return 21;
				else if(missGrad && masslessOnly)
					return 24;
				else if(!missGrad && !masslessOnly)
					return 23;
				else
					return 20;
			} else {
				if(missGfsr && !missGrad && masslessOnly)
					return 10;
				else if(missGrad && !masslessOnly)
					return 11;
				else if(missGrad && masslessOnly)
					return 4;
				else if(!missGrad && !masslessOnly)
					return 3;
				else
					return -20;
			}
		} else {
			std::cout << "[GenInfo] compareFinalStates: At least one particle should be missing!" << std::endl;
			return -8;
		}
	} else {
		std::cout << "[GenInfo] compareFinalStates: More reconstructed than generated particle in final state!" << std::endl;
		return -5;
	}
}

//TODO
// Flags
// -1 - none of the below
//  0 - neither of the two gammas point to pi0
//  1 - correctly reconstructed pi0
//  2 - only one gamma has mother (CAUTION) - link is made to that pi0
//  3 - only one gamma has link to pi0 (CAUTION), the other gamma links to something else - link is made to that pi0
//  4 - mothers of gammas are different pi0s (CAUTION) - link is made to mother of gamma with largest energy
//  5 - only one gamma has link to gen_ehepvt - link is made to mother of gamma, if exists
int getMCtruthPi0Flag(Particle &p) {
	// if link is not to pi0
	if(IDhep(p)!=111)
		return 0;

	Gen_hepevt_Manager& GenMgr = Gen_hepevt_Manager::get_manager();
	if(p.child(0).genHepevt() && p.child(1).genHepevt()) {
		const Gen_hepevt & mother1(p.child(0).genHepevt());
		const Gen_hepevt & mother2(p.child(1).genHepevt());
		if(mother1.mother() && mother2.mother()) {
			if(mother1.mother().get_ID()==mother2.mother().get_ID()) {
				if(mother1.mother().get_ID()==0) {
					std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
					return 0;
				}
				return 1;
			} else if(mother1.mother().idhep()==111 && mother2.mother().idhep()==111) {
				if(p.child(0).e()>p.child(1).e()) {
					if(mother1.mother().get_ID()==0) {
						std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
						return 0;
					}
					return 4;
				} else {
					if(mother2.mother().get_ID()==0) {
						std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
						return 0;
					}
					return 4;
				}
			} else if(mother1.mother().idhep()==111 || mother2.mother().idhep()==111) {
				if(mother1.mother().idhep()==111) {
					if(mother1.mother().get_ID()==0) {
						std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
						return 0;
					}
					return 3;
				} else {
					if(mother2.mother().get_ID()==0) {
						std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
						return 0;
					}
					return 3;
				}
			}
		} else {
			if(mother1.mother()) {
				if(mother1.mother().get_ID()==0) {
					std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
					return 0;
				}
				return 2;
			} else if(mother2.mother()) {
				if(mother2.mother().get_ID()==0) {
					std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
					return 0;
				}
				return 2;
			}
		}
	} else if(p.child(0).genHepevt()) {
		const Gen_hepevt & mother(p.child(0).genHepevt());
		if(mother.mother()) {
			if(mother.mother().idhep()==111) {
				if(mother.mother().get_ID()==0) {
					std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
					return 0;
				}
				return 5;
			}
			return 0;
		}
	} else if(p.child(1).genHepevt()) {
		const Gen_hepevt & mother(p.child(1).genHepevt());
		if(mother.mother()) {
			if(mother.mother().idhep()==111) {
				if(mother.mother().get_ID()==0) {
					std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
					return 0;
				}
				return 5;
			}
			return 0;
		}
	} else {
		std::cout << "[GenInfo] setMCtruthPi0: Neither of two photons has link to gen_hepevt!" << std::endl;
	}

	return -1;
}


void setMCtruthPi0(Particle &p) {
	// MC truth already set
	if(IDhep(p)!=0) {
		return;
	}

	for(int i=0; i<(int)p.nChildren(); ++i) {
		if(p.child(i).mdstGamma()) {
			const Gen_hepevt & hep(gen_level(get_hepevt(p.child(i).mdstGamma())));
			if (hep){
				p.child(i).relation().genHepevt(hep);
			} else {
				std::cout << " [GenInfo] setMCtruthPi0: child " << i << " has no link! " << std::endl;
			}
		} else {
			std::cout << " [GenInfo] setMCtruthPi0: pi0 child is not mdstGamma! Exit. " << std::endl;
		}
	}

	Gen_hepevt_Manager& GenMgr = Gen_hepevt_Manager::get_manager();
	if(p.child(0).genHepevt() && p.child(1).genHepevt()) {
		const Gen_hepevt & mother1(p.child(0).genHepevt());
		const Gen_hepevt & mother2(p.child(1).genHepevt());
		if(mother1.mother() && mother2.mother()) {
			if(mother1.mother().get_ID()==mother2.mother().get_ID()) {
				if(mother1.mother().get_ID()==0) {
					std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
					return;
				}
				const Gen_hepevt& thisGen = GenMgr(Panther_ID(mother1.mother().get_ID()));
				p.relation().genHepevt(thisGen);
				return;
			} else if(mother1.mother().idhep()==111 && mother2.mother().idhep()==111) {
				if(p.child(0).e()>p.child(1).e()) {
					if(mother1.mother().get_ID()==0) {
						std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
						return;
					}
					const Gen_hepevt& thisGen = GenMgr(Panther_ID(mother1.mother().get_ID()));
					p.relation().genHepevt(thisGen);
					return;
				} else {
					if(mother2.mother().get_ID()==0) {
						std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
						return;
					}
					const Gen_hepevt& thisGen = GenMgr(Panther_ID(mother2.mother().get_ID()));
					p.relation().genHepevt(thisGen);
					return;
				}
			} else if(mother1.mother().idhep()==111 || mother2.mother().idhep()==111) {
				if(mother1.mother().idhep()==111) {
					if(mother1.mother().get_ID()==0) {
						return;
					}
					const Gen_hepevt& thisGen = GenMgr(Panther_ID(mother1.mother().get_ID()));
					p.relation().genHepevt(thisGen);
					return;
				} else {
					if(mother2.mother().get_ID()==0) {
						std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
						return;
					}
					const Gen_hepevt& thisGen = GenMgr(Panther_ID(mother2.mother().get_ID()));
					p.relation().genHepevt(thisGen);
					return;
				}
			}
		} else {
			if(mother1.mother()) {
				if(mother1.mother().get_ID()==0) {
					std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
					return;
				}
				const Gen_hepevt& thisGen = GenMgr(Panther_ID(mother1.mother().get_ID()));
				p.relation().genHepevt(thisGen);
				return;
			} else if(mother2.mother()) {
				if(mother2.mother().get_ID()==0) {
					std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
					return;
				}
				const Gen_hepevt& thisGen = GenMgr(Panther_ID(mother2.mother().get_ID()));
				p.relation().genHepevt(thisGen);
				return;
			}
		}
	} else if(p.child(0).genHepevt()) {
		std::cout << "[GenInfo] setMCtruthPi0: Only first photon with link to gen_hepevt: " << std::endl;
		const Gen_hepevt & mother(p.child(0).genHepevt());
		std::cout << " -> idhep = " << mother.idhep() << std::endl;
		if(mother.mother()) {
			std::cout << " -> mother ID = " << mother.mother().idhep() << std::endl;
			if(mother.mother().idhep()==111) {
				if(mother.mother().get_ID()==0) {
					std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
					return;
				}
				const Gen_hepevt& thisGen = GenMgr(Panther_ID(mother.mother().get_ID()));
				p.relation().genHepevt(thisGen);
				return;
			}
			return;
		}
	} else if(p.child(1).genHepevt()) {
		std::cout << "[GenInfo] setMCtruthPi0: Only second photon with link to gen_hepevt: " << std::endl;
		const Gen_hepevt & mother(p.child(1).genHepevt());
		std::cout << " -> idhep = " << mother.idhep() << std::endl;
		if(mother.mother()) {
			std::cout << " -> mother ID = " << mother.mother().idhep() << std::endl;
			if(mother.mother().idhep()==111) {
				if(mother.mother().get_ID()==0) {
					std::cout << "[GenInfo] setMCtruthPi0: requesting particle with ID = 0! Exiting." << std::endl;
					return;
				}
				const Gen_hepevt& thisGen = GenMgr(Panther_ID(mother.mother().get_ID()));
				p.relation().genHepevt(thisGen);
				return;
			}
			return;
		}
	} else {
		std::cout << "[GenInfo] setMCtruthPi0: Neither of two photons has link to gen_hepevt!" << std::endl;
	}


	return;
}

// Flags
// 0 - link doesn't exist
// 1 - correctly identified charged track
// 2 - misidentified charged track
int getMCtruthChargedTrackFlag(Particle &p) {
	if(IDhep(p)==0)
		return 0;


	if(p.lund()==IDhep(p)) {
		return 1;
	} else {
		return 2;
	}
}

void setMCtruth(Particle &p){
	// MC truth already set
	if(IDhep(p)!=0) {
		return;
	}

	if(p.lund()==111) {
		setMCtruthPi0(p);
		return;
	}

	if(p.mdstCharged()) {
		const Gen_hepevt & hep(gen_level(get_hepevt(p.mdstCharged())));
		if (hep){
			p.relation().genHepevt(hep);
			return;
		}
	} else if(p.mdstGamma()) {
		const Gen_hepevt & hep(gen_level(get_hepevt(p.mdstGamma())));
		if (hep){
			p.relation().genHepevt(hep);
			return;
		}
	}

	// it is not mdstCharged or mdstGamma (combined particle)
	int nChildren = p.relation().nChildren();
	if(nChildren<2)
		return;

	// special treatment for pi0
	// in case that one of the gammas doesn't have link to gen_hepevt but the otherone has
	// set link for pi0 to the mother of that gamma

	// check that all child particles have MC truth
	for(int i=0; i<nChildren; ++i) {
		if(!p.relation().child(i).genHepevt())
			setMCtruth(p.relation().child(i));

		if(!p.relation().child(i).genHepevt()) {
			return;
		}
		if(p.relation().child(i).genHepevt().idhep() == 0) {
			return;
		}
	}

	// check that there are no clones
	for(int i=0; i<nChildren; ++i) {
		for(int j=i+1; j<nChildren; ++j) {
			if(p.relation().child(i).genHepevt().get_ID() == p.relation().child(j).genHepevt().get_ID()) {
				return;
			}
		}
	}

	// find common mother, if exists (p has at least 2 children)
	std::vector<int> thisMothers;
	std::vector<int> otherMothers;

	fillMothers(p, thisMothers, otherMothers);
	if(thisMothers.size()==0 || otherMothers.size()==0) {
		return;
	}
	int motherID =  findCommonMother(p.nChildren(), thisMothers, otherMothers);

	if(!motherID) {
		std::cout << p.lund() <<"[GenInfo] setMCtruth: requesting particle with ID = 0! Exiting." << std::endl;
		return;
	}

	Gen_hepevt_Manager& GenMgr = Gen_hepevt_Manager::get_manager();
	const Gen_hepevt& thisGen = GenMgr(Panther_ID(motherID));
	// setting the relation
	p.relation().genHepevt(thisGen);
}

	// number and type of final state particles should be the same
    // Flags
    // -11 : gen_hepevt link of one of children not found in list of daughters of matched gen_hepevt (something wrong)
    // -10 : one of children has not link to gen_hepevt (something wrong)
    //  -9 : as -10 but in addition size of reconstructed < size of generated
    //  -5 : more reconstructed than generated particle in final state (something wrong)
    //  -2 : gen_hepvt particle doesn't have any daughters (something wrong in maching)
    //  -1 : random combination (common mother is virtual gamma (ccbar, uds mc), or Upsilon(4S), Upsilon(5S))
    //   0 : no link to gen_hepevt particle
	//   1 : particle is correctly reconstructed; including correct particle id of final state particles (SIGNAL)
	//   2 : one or more FSP are misidentified, but have common mother
	//   3 : FSP have common mother, but at least one massive particle is missing
	//   4 : FSP have common mother, but at least one massless particle is missing (radiative photon)
	//   5 : final state includes pi0 without perfect match to gen_hepevt
	//   6 : ID = 2 and 5 are true
    //  10 : particle is correctly reconstructed; including correct particle id of final state particles, but FSR photon is missing (SIGNAL)
    //  11 : in addition to FSR photon missing one more radiative photon is missing
    //  20 : missing neutrino
    //  21 : missing neutrino and radiative photon
	//  23 : missing neutrino and massive particle
    //  24 : missing neutrino and another massles particle (FSR photon)

int getMCtruthFlag(Particle &p) {
	if(IDhep(p)==0)
		return 0;

	if(p.lund()==111) 
	  return getMCtruthPi0Flag(p);

	if(p.mdstCharged())
		return getMCtruthChargedTrackFlag(p);

	Gen_hepevt thisGen = p.relation().genHepevt();

	int motherIDhep = thisGen.idhep();
	if(motherIDhep==10022 || motherIDhep==300553 || motherIDhep==9000553) {
		return -1;
	}

	// Ks doesn't have daughters in gen_hepevt table
	if(motherIDhep==310) {
		return 1;
	}

	std::vector<Particle>   reconstructed;
	std::vector<Gen_hepevt> generated;

	appendRecFSP(p, reconstructed);
	int a = appendGenFSP(thisGen, generated);
	if(a==0) {
		return -2;
	}

	int truth = compareFinalStates(reconstructed, generated);
	return truth;
}



void setMCtruth(std::vector<Particle> &plist){
  if(plist.size() == 0) return;

  for(std::vector<Particle>::iterator i = plist.begin();
      i != plist.end(); ++i){
    setMCtruth(*i);
  }
}
void fillMothers(Particle &A, Particle &B, std::vector<int> &thisMothers, std::vector<int> &otherMothers) {
	Gen_hepevt motherThis(A.genHepevt());

	while(motherThis.mother()) {
		motherThis = motherThis.mother();
		thisMothers.push_back(motherThis.get_ID());
	}

	motherThis = B.genHepevt();
	while(motherThis.mother()) {
		motherThis = motherThis.mother();
		otherMothers.push_back(motherThis.get_ID());
	}
}

void fillMothers(Particle &A, Particle &B, Particle &C, std::vector<int> &thisMothers, std::vector<int> &otherMothers) {
	Gen_hepevt motherThis(A.genHepevt());

	while(motherThis.mother()) {
		motherThis = motherThis.mother();
		thisMothers.push_back(motherThis.get_ID());
	}

	motherThis = B.genHepevt();
	while(motherThis.mother()) {
		motherThis = motherThis.mother();
		otherMothers.push_back(motherThis.get_ID());
	}

	motherThis = C.genHepevt();
	while(motherThis.mother()) {
		motherThis = motherThis.mother();
		otherMothers.push_back(motherThis.get_ID());
	}
}

int getCommonMother(Particle &A, Particle &B) {
	if(!A.genHepevt())
		return 0;
	if(!B.genHepevt())
		return 0;

	std::vector<int> thisMothers;
	std::vector<int> otherMothers;

	fillMothers(A, B, thisMothers, otherMothers);
	if(thisMothers.size()==0 || otherMothers.size()==0) {
		return 0;
	}

	int motherID = findCommonMother(2, thisMothers, otherMothers);

	if(!motherID) {
		std::cout << "[GenInfo] setMCtruth: requesting particle with ID = 0! Exiting." << std::endl;
		return 0;
	}

	Gen_hepevt_Manager& GenMgr = Gen_hepevt_Manager::get_manager();
	const Gen_hepevt& thisGen = GenMgr(Panther_ID(motherID));

	return thisGen.idhep();
}

int getCommonMother(Particle &A, Particle &B, Particle &C) {
	if(!A.genHepevt())
		return 0;
	if(!B.genHepevt())
		return 0;
	if(!C.genHepevt())
		return 0;

	std::vector<int> thisMothers;
	std::vector<int> otherMothers;

	fillMothers(A, B, C, thisMothers, otherMothers);
	if(thisMothers.size()==0 || otherMothers.size()==0) {
		return 0;
	}

	int motherID =  findCommonMother(3, thisMothers, otherMothers);

	if(!motherID) {
		std::cout << "[GenInfo] setMCtruth: requesting particle with ID = 0! Exiting." << std::endl;
		return 0;
	}

	Gen_hepevt_Manager& GenMgr = Gen_hepevt_Manager::get_manager();
	const Gen_hepevt& thisGen = GenMgr(Panther_ID(motherID));

	return thisGen.idhep();
}

////////////////////////////////////////////////////////////////////////////////
// get decay chain for final stat particle

void genDecayChain(Particle p, int* dChain) {
	for(int i=0; i<=8; i++) dChain[i] = -1;

	if(p.relation().genHepevt()) {
		Gen_hepevt igen = p.relation().genHepevt();
		dChain[0] = igen.idhep();

		Gen_hepevt imot = igen.mother();
		if(imot) {
			dChain[1] = imot.idhep();
			dChain[2] = imot.daLast()-imot.daFirst()+1;

			Gen_hepevt immot = imot.mother();
			if(immot) {
				dChain[3] = immot.idhep();
				dChain[4] = immot.daLast()-immot.daFirst()+1;

				Gen_hepevt rg_mmmot = immot.mother();
				if(rg_mmmot) {
					dChain[5] = rg_mmmot.idhep();
					dChain[6] = rg_mmmot.daLast()-rg_mmmot.daFirst()+1;

					Gen_hepevt mrg_mmmot = rg_mmmot.mother();
					if(mrg_mmmot) {
						dChain[7] = mrg_mmmot.idhep();
						dChain[8] = mrg_mmmot.daLast()-mrg_mmmot.daFirst()+1;
					}
				}
			}
		}
	}
}

int IDhep(Particle &part) {

  if(! part.genHepevt()) return 0;
  return part.genHepevt().idhep();
}

int NdecayProd(Particle &part) {

  if(! part.genHepevt()) return 0;
  return part.genHepevt().daLast() - part.genHepevt().daFirst() +1;
}


#if defined(BELLE_NAMESPACE)
} // namespace Belle
#endif
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 GenParticleInfo.cc                                                                                  0000644 0047266 0051066 00000000244 12770400276 014102  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "GenParticleInfo.h"

ClassImp(GenParticleInfo)

void GenParticleInfo::Clear(void){
  id   = -99;
  flag = -99;
  for(int i=0; i<9; i++) chain[i] = -99;
}

                                                                                                                                                                                                                                                                                                                                                            H0Info.cc                                                                                           0000644 0047266 0051066 00000001015 13064453236 012152  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "H0Info.h"

#include <cmath>

using std::sin;
using std::cos;
using std::sqrt;

ClassImp(H0Info)

void H0Info::Clear(void) {
    p = 0., costh = 0., phi = 0.;  // Momentum
    mchi2 = 0.;
//    vchi2 = 0.;
    m = 0.;
}

double H0Info::pz(void) const { return p * costh;}
double H0Info::pt(void) const { return p * sqrt(1. - costh * costh);}
double H0Info::px(void) const { return pt() * cos(phi);}
double H0Info::py(void) const { return pt() * sin(phi);}

#ifdef __MAKECINT__
#pragma link C++ class H0Info;
#endif

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   HamletTag.cc                                                                                        0000644 0047266 0051066 00000000226 12770400276 012737  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "HamletTag.h"

ClassImp(HamletTag)

void HamletTag::Clear(void){
  tag_LH = -2, tag_LH_err = -2; //
  tagl = -2, taglid = -2;
  flv = -2;
}

                                                                                                                                                                                                                                                                                                                                                                          IPBoost.cc                                                                                          0000644 0047266 0051066 00000000250 12770400276 012405  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "IPBoost.h"

ClassImp(IPBoost)

void IPBoost::Clear(void){
  ip[0] = -99; ip[1] = -99; ip[2] = -99; //
  boost[0] = -99; boost[1] = -99; boost[2] = -99; //
}

                                                                                                                                                                                                                                                                                                                                                        kinefitter.cc                                                                                       0000644 0047266 0051066 00000024471 12770400276 013245  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "kinefitter.h"

#include "belle.h"
#include "exkfitter/ExKFitter.h"
#include "kfitter/kmassfitter.h"
//#include "ip/IpProfile.h"
#include "kfitter/kmakemother.h"
#include "particle/utility.h"
#include "uisetter.h"

using namespace std;
#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif

kvertexfitter KineFitter::vtx_fit(Particle& p,const bool wIPtube,int& flag){
  kvertexfitter kv_rec;
  vector<Particle*> chl = p.relation().children();
  for(int i=0; i<chl.size(); i++) addTrack2fit(kv_rec,*chl[i]);

  if(!IpProfile::usable()) cout << "Unusable IP tube" << endl;
  else if(wIPtube) addTube2fit(kv_rec);
  flag = kv_rec.fit();
  return kv_rec;
}

kmassvertexfitter KineFitter::mass_vtx_fit(Particle& p){
  kmassvertexfitter kmv_rec;
  vector<Particle*> chl = p.relation().children();
  for(int i=0; i<chl.size(); i++) addTrack2fit(kmv_rec,*chl[i]);
  kmv_rec.invariantMass(p.pType().mass());
  kmv_rec.fit();
  return kmv_rec;
}

kmassfitter KineFitter::mass_fit(Particle& p, const bool atvtx){
  kmassfitter km_rec;
  vector<Particle*> chl = p.relation().children();
  for(int i=0; i<chl.size(); i++) addTrack2fit(km_rec,*chl[i]);
  km_rec.invariantMass(p.pType().mass());
  if(atvtx){
    km_rec.atDecayPoint();
    km_rec.vertex(p.x());
  }
  km_rec.fit();
  return km_rec;
}

int KineFitter::vtx_fit_r(Particle& p,double& chisq){
  kvertexfitter kv_rec;
  vector<Particle*> chl = p.relation().children();
  for(int i=0; i<chl.size(); i++){
    Particle& ch = *chl[i];
    if(ch.nChildren()>0){
      double chi2;
      if(KineFitter::vtx_fit_r(ch,chi2)) return -1;
    }
    addTrack2fit(kv_rec,ch);
  }
  const int err = kv_rec.fit();
  if(err){
    cout << "KineFitter::mass_fit_r: fail" << endl;
    return err;
  } else{
    const int err1 = make_mother(p,kv_rec);
    if(err1) return err1;
  }
  chisq = kv_rec.chisq()/kv_rec.dgf();
  return 0;
}

int KineFitter::mass_fit_r(Particle& p,double& chisq){
  kmassfitter km_rec;
  km_rec.invariantMass(p.pType().mass());
  vector<Particle*> chl = p.relation().children();
  for(int i=0; i<chl.size(); i++){
    Particle& ch = *chl[i];
    if(ch.nChildren()>0){
      double chi2;
      if(KineFitter::mass_fit_r(ch,chi2)) return -1;
    }
    addTrack2fit(km_rec,ch);
  }
  const int err = km_rec.fit();
  if(err){
    cout << "KineFitter::mass_fit_r: fail" << endl;
    return err;
  } else{
    const int err1 = make_mother(p,km_rec);
    if(err1) return err1;
  }
  chisq = km_rec.chisq()/km_rec.dgf();
  return 0;
}

int KineFitter::make_mother(Particle& p, kvertexfitter& kf){
  kmakemother kmm;
  makeMother(kmm,kf,p,1);
  const int err = kmm.make();
  if(err){
    cout << "KineFitter: fail in kmakemother" << endl;
    return err;
  } else{
    p.momentum().momentumPosition(kmm.momentum(), // set information.
                                  kmm.position(), // 4-momentum, position and these error.
                                  kmm.error());
  }
  return 0;
}

int KineFitter::make_mother(Particle& p, kmassfitter& kf){
  kmakemother kmm;
  makeMother(kmm,kf,p,1);
  const int err = kmm.make();
  if(err){
    cout << "KineFitter: fail in kmakemother" << endl;
    return err;
  } else{
    p.momentum().momentumPosition(kmm.momentum(), // set information.
                                  kmm.position(), // 4-momentum, position and these error.
                                  kmm.error());
  }
  return 0;
}

//  kmakemother kmm;
//  kmm.addTrack(Pip.Momentum(),Pip.Position(),Pip.ErrMomentumPosition(),pip.pType().charge());
//  kmm.addTrack(Pim.Momentum(),Pim.Position(),Pim.ErrMomentumPosition(),pim.pType().charge());
//  kmm.addTrack(Ks.Momentum(),Ks.Position(),kmmks.error(),d0.child(0).pType().charge());
//  kmm.vertex(D0.Vertex()->Vertex());
//  kmm.errVertex(D0.Vertex()->ErrVertex());
//  if(kmm.make()) cout << "kmm failed" << endl;

//  d0.momentum().momentumPosition(D0.Momentum(),D0.Position(),kmm.error());
//  d0.momentum().decayVertex(D0.Vertex()->Vertex(),D0.Vertex()->ErrVertex());

int KineFitter::b0tod0pipi_pipivtx_fit(Particle& b0){
  kvertexfitter kv_rec;
  addTrack2fit(kv_rec,b0.child(1));
  addTrack2fit(kv_rec,b0.child(2));
  if(!IpProfile::usable()) cout << "Unusable IP tube" << endl;
  else addTube2fit(kv_rec);
  const int status = kv_rec.fit();
  if(!status) UISetter::SetSigVtxFitInfo(b0,kv_rec);
  return status;
}

int KineFitter::b0tod0pipi_d0vtx_fit(Particle& b0){
  kvertexfitter kv_rec;
  int d0index = 0;
       if(abs(b0.child(0).lund()) == 421) d0index = 0;
  else if(abs(b0.child(1).lund()) == 421) d0index = 1;
  else if(abs(b0.child(2).lund()) == 421) d0index = 2;
  else return -9;

  addTrack2fit(kv_rec,b0.child(d0index));
  if(!IpProfile::usable()){
    cout << "Unusable IP tube" << endl;
    return -1;
  }
  else addTube2fit(kv_rec);
  const int status = kv_rec.fit();
  if(!status) UISetter::SetSigVtxFitInfo(b0,kv_rec);
  return status;
}

int KineFitter::d0tokspipi_vtx_fit(Particle& d0){
// Ks0 vertex fit
  kvertexfitter kv_rec_ks;
  addTrack2fit(kv_rec_ks,d0.child(0).child(0));
  addTrack2fit(kv_rec_ks,d0.child(0).child(1));
  const int unusable_ks = kv_rec_ks.fit();
  Particle& ks = d0.child(0);
  if(unusable_ks){
    std::cout << "!!! Unusable Ks0" << std::endl;
  } else{
    makeMother(kv_rec_ks,ks);
  }

  kvertexfitter kv_rec;
  addTrack2fit(kv_rec,d0.child(1));
  addTrack2fit(kv_rec,d0.child(2));
  if(!unusable_ks) addTrack2fit(kv_rec,ks);
  if(kv_rec.fit()) return -1;
  dynamic_cast<D0UserInfo&>(d0.userInfo()).VtxChi2(kv_rec.chisq()/kv_rec.dgf());
  dynamic_cast<D0UserInfo&>(d0.userInfo()).VtxFitDone();
  if(!unusable_ks){
    makeMother(kv_rec,d0);
    dynamic_cast<D0UserInfo&>(d0.userInfo()).VtxFitGood(true);
  }
  return 0;
}
/*
int KineFitter::DTF(Particle& p){
  const double m_p = p.pType().mass();
  ExKFitterMass M_p(m_p);
  ExKFitterVertex V_p(p.x());
  ExKFitterParticle P;
  ExKFitterConstrain PVertC;
  ExKFitterConstrain PMassC;
  PVertC.SetVertexConstrain();
  PMassC.SetMassConstrain();

  vector<Particle*> p_chl = p.relation().children();
  for(int i=0; i<p_chl.size(); i++){// children, level 1
    Particle& pg1 = *p_chl[i];
    ExKFitterParticle* PG1;
    if(pg1.nChildren() != 0){
      vector<Particle*> p_chl_l2 = pg1.relation().children();
      for(int i2; i2<p_chl_l2.size(); i2++){// children, level 2
        
      }
    } else{
      PG1 = new ExKFitterParticle(pg1.p(),pg1.x(),pg1.momentum().dpx(),pg1.charge());
    }
    P.LinkParticle(PG1);
    PVertC.
  }

  PVertC.LinkVertex(V_p);
  PMassC.LinkVertex(V_p);
  PMassC.LinkMass(M_p);

}
*/

int KineFitter::d0tokspipi_dtf(Particle& d0){
  const double d0m2_before = d0.p().m2();
  Particle& pip = const_cast<Particle&>(d0.child(1));
  Particle& pim = const_cast<Particle&>(d0.child(2));
  Particle& ks0_pi1 = const_cast<Particle&>(d0.child(0).child(0));
  Particle& ks0_pi2 = const_cast<Particle&>(d0.child(0).child(1));

  ExKFitterParticle Pip(pip.p(),pip.x(),pip.momentum().dpx(),pip.charge());
  ExKFitterParticle Pim(pim.p(),pim.x(),pim.momentum().dpx(),pim.charge());
  ExKFitterParticle KsPi1(ks0_pi1.p(),ks0_pi1.x(),ks0_pi1.momentum().dpx(),ks0_pi1.charge());
  ExKFitterParticle KsPi2(ks0_pi2.p(),ks0_pi2.x(),ks0_pi2.momentum().dpx(),ks0_pi2.charge());

  ExKFitterMass K_mass(0.497614);
  ExKFitterMass D_mass(1.86484);

  ExKFitterVertex KsVtx(d0.child(0).x());
  ExKFitterVertex D0Vtx(d0.x());

  ExKFitterParticle Ks;
  Ks.LinkParticle(&KsPi1);
  Ks.LinkParticle(&KsPi2);
  Ks.LinkVertex(&KsVtx);

  ExKFitterConstrain KsVertConstr;
  KsVertConstr.SetVertexConstrain();
  KsVertConstr.LinkParticle(&KsPi1);
  KsVertConstr.LinkParticle(&KsPi2);
  KsVertConstr.LinkVertex(&KsVtx);

  ExKFitterConstrain KsMassConstr;
  KsMassConstr.SetMassConstrain();
  KsMassConstr.LinkParticle(&KsPi1);
  KsMassConstr.LinkParticle(&KsPi2);
  KsMassConstr.LinkVertex(&KsVtx);
  KsMassConstr.LinkMass(&K_mass);

  ExKFitterParticle D0;
  D0.LinkParticle(&Ks);
  D0.LinkParticle(&Pip);
  D0.LinkParticle(&Pim);
  D0.LinkVertex(&D0Vtx);

  ExKFitterConstrain D0VertConstr;
  D0VertConstr.SetVertexConstrain();
  D0VertConstr.LinkParticle(&Ks);
  D0VertConstr.LinkParticle(&Pip);
  D0VertConstr.LinkParticle(&Pim);
  D0VertConstr.LinkVertex(&D0Vtx);

  ExKFitterConstrain D0MassConstr;
  D0MassConstr.SetMassConstrain();
  D0MassConstr.LinkParticle(&Ks);
  D0MassConstr.LinkParticle(&Pip);
  D0MassConstr.LinkParticle(&Pim);
  D0MassConstr.LinkVertex(&D0Vtx);
  D0MassConstr.LinkMass(&D_mass);

  ExKFitter Core;
  Core.LinkConstrain(&KsVertConstr);
  Core.LinkConstrain(&KsMassConstr);
  Core.LinkConstrain(&D0VertConstr);
  Core.LinkConstrain(&D0MassConstr);

  const int status = Core.Minimize();

  if(status<0){
    return status;
  }

  D0.Update();
  const double d0m2_after = d0.p().m2();
  if(abs(d0m2_after-d0m2_before)>0.001) cout << "D0 mass changed in mass constrained fit!!!" << endl;
  dynamic_cast<D0UserInfo&>(d0.userInfo()).MassChi2(Core.Chisq()/Core.N_DegreeOfFreedom());
  const HepLorentzVector pkpip = Pip.Momentum() + Ks.Momentum();
  const HepLorentzVector pkpim = Pim.Momentum() + Ks.Momentum();
  const double mp = pkpip.m2();
  const double mm = pkpim.m2();
  dynamic_cast<D0UserInfo&>(d0.userInfo()).SetDalitzVars(mp,mm);
//  dynamic_cast<D0UserInfo&>(d0.userInfo()).md0_fit(D0.Momentum().m());

  return status;
}

//======
// Set proper error matrix for gamma. Note errCart length is 4.
// Copied from FindGamma in icpv_skim package.
//======
void KineFitter::errGam(HepSymMatrix& errCart,const Mdst_gamma& gamma){
//^^^^^^^^^^^^^ output , ^^^^^^^^^^ input
  HepSymMatrix  errEcl(3,0); // 3x3 initialize to zero
  errEcl[0][0] = gamma.ecl().error(0); // Energy
  errEcl[1][0] = gamma.ecl().error(1);
  errEcl[1][1] = gamma.ecl().error(2); // Phi
  errEcl[2][0] = gamma.ecl().error(3);
  errEcl[2][1] = gamma.ecl().error(4);
  errEcl[2][2] = gamma.ecl().error(5); // Theta

  HepMatrix jacobian(4,3,0);
  double cp = cos(gamma.ecl().phi());
  double sp = sin(gamma.ecl().phi());
  double ct = cos(gamma.ecl().theta());
  double st = sin(gamma.ecl().theta());
  double  E = gamma.ecl().energy();

  jacobian[0][0] =    cp*st;
  jacobian[0][1] = -E*sp*st;
  jacobian[0][2] =  E*cp*ct;
  jacobian[1][0] =    sp*st;
  jacobian[1][1] =  E*cp*st;
  jacobian[1][2] =  E*sp*ct;
  jacobian[2][0] =       ct;
  jacobian[2][1] =      0.0;
  jacobian[2][2] = -E   *st;
  jacobian[3][0] =      1.0;
  jacobian[3][1] =      0.0;
  jacobian[3][2] =      0.0;
  errCart = errEcl.similarity(jacobian);
  return;
}

int pi0_mass_fit(Particle& pi0){

}

#if defined(BELLE_NAMESPACE)
}
#endif

                                                                                                                                                                                                       Ks0Info.cc                                                                                          0000644 0047266 0051066 00000000255 12770400276 012344  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "Ks0Info.h"

ClassImp(Ks0Info)

void Ks0Info::Clear(void){
  m = 0, m_raw = 0;
  p[0] = -99, p[1] = -99, p[2] = -99;
  dr = -99, dz = -99, dphi = -99, fl = -99;
}

                                                                                                                                                                                                                                                                                                                                                   ksfwmoments.cc                                                                                      0000644 0047266 0051066 00000027471 12770400276 013461  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                
	      //////////////////////////////////////////
	      //                                      //
	      //  ksfwmoments.cc                      //
	      //                                      //
	      //  moment-calculation of the k_sfw     //
	      //  improved Super-Fox-Wolfram moments  //
	      //  to be used with rooksfw.{cc,h}      //
	      //                                      //
	      //  M. Nakao                            //
	      //                                      //
	      //////////////////////////////////////////

// Versions
// 2006111301  first version


#include <stdio.h>
#include <unistd.h>
#include <ip/IpProfile.h>
#include <particle/Particle.h>
#include <kfitter/kvertexfitter.h>
#include <mdst/findKs.h>
#include MDST_H

#include "ksfwmoments.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif

// ----------------------------------------------------------------------
// legendre
// ----------------------------------------------------------------------
inline double
legendre(const double z, const int i)
{
  switch (i) {
  case 0:  return 1.;
  case 1:  return z;
  case 2:  return 1.5*z*z - 0.5;
  case 3:  return z*(2.5*z*z - 1.5);
  case 4:  return (4.375*z*z*z*z - 3.75*z*z + 0.375);
  //(wrong) case 4:  return (4.735*z*z*z*z - 3.75*z*z + 0.375);
  default: return 0;
  }
}
// ----------------------------------------------------------------------
// constructor
// (copied from k_sfw.cc with minimum modification)
// ----------------------------------------------------------------------
ksfwmoments::ksfwmoments(const Particle &cand, double ebeam,
			 const Hep3Vector &cmboost)
{
  if (! IpProfile::usable()) {
    printf("======================================================\n");
    printf("rooksfw: IpProfile::begin_run() has not been called!!!\n");
    printf("rooksfw: IpProfile is not usuable!!!\n");
    printf("======================================================\n");
    exit(1);
  }
  
  // kinematically allowed maximum momentum for mbc>5.2
  // sqrt(5.29^2 - 5.2^2) ~ 1.0GeV
  // 10.58/4 + 1.0/2 = 3.145
  static const double P_MAX(3.2);
  static const double PIMASS(0.139570);

  //=====================
  // create particle list
  //=====================
  HepLorentzVector pb(cand.p());
  pb.boost(cmboost);
  const double Hso0_max(2*(2*ebeam-pb.e())); // 2 * (ebeam - delta_E)

  std::vector<Hep3Vector> p_cms_sigA, p_cms_sigB, p_cms_other;
  std::vector<int> Q_sigA, Q_sigB, Q_other;
  HepLorentzVector p_cms_missA(0, 0, 0, 2*ebeam);
  HepLorentzVector p_cms_missB(0, 0, 0, 2*ebeam);
  m_et[0] = m_et[1] = 0;

  Mdst_vee2_Manager& veeMgr(Mdst_vee2_Manager::get_manager());
  Mdst_charged_Manager& chMgr(Mdst_charged_Manager::get_manager());
  Mdst_gamma_Manager& gMgr(Mdst_gamma_Manager::get_manager());
  const std::vector<Particle *> & final 
    = cand.relation().finalStateParticles();

  // for signal (use_finalstate_for_sig == 0)
  for (int i = 0; i < cand.nChildren(); i++){
    const Particle & dau(cand.child(i));
    HepLorentzVector p_cms(dau.p());
    p_cms.boost(cmboost);
    p_cms_sigA.push_back(p_cms.vect());
    Q_sigA.push_back((int)dau.charge());
    p_cms_missA -= p_cms;
    m_et[0] += p_cms.perp();
  }

  // ks
  FindKs fKs;
  std::vector<Mdst_charged *> ks_dau;
  for (std::vector<Mdst_vee2>::iterator i = veeMgr.begin();
       i != veeMgr.end(); i++){
    bool sig(false);
    for (std::vector<Particle *>::const_iterator j = final.begin();
	 j != final.end(); j++){
      if ((**j).mdstCharged() && 
	  ((**j).mdstCharged().get_ID() == (*i).chgd(0).get_ID() || 
	   (**j).mdstCharged().get_ID() == (*i).chgd(1).get_ID() )){
	sig = true;
	break;
      }
    }
    if (sig) continue;
    if ( (*i).kind() != 1 ) continue;
    fKs.candidates( *i, IpProfile::position() );
    if( !fKs.goodKs() ) continue;
    HepLorentzVector p_cms((*i).px(), (*i).py(), (*i).pz(), (*i).energy());
    p_cms.boost(cmboost);
    if (p_cms.rho() > P_MAX) continue;
    p_cms_other.push_back(p_cms.vect());
    Q_other.push_back(0);
    ks_dau.push_back(&(*i).chgd(0));
    ks_dau.push_back(&(*i).chgd(1));
    p_cms_missA -= p_cms;
    p_cms_missB -= p_cms;
    m_et[0] += p_cms.perp();
    m_et[1] += p_cms.perp();
  }

  // charged tracks
  HepVector a(5);
  HepSymMatrix Ea(5,0);
  for (std::vector<Mdst_charged>::const_iterator i = chMgr.begin();
       i != chMgr.end(); i++){
    bool ksdau(false);
    for (std::vector<Mdst_charged *>::const_iterator j = ks_dau.begin();
	 j != ks_dau.end(); j++){
      if ((**j).get_ID() == (*i).get_ID()){
	ksdau = true;
	break;
      }
    }
    if (ksdau) continue;
    bool sig(false);
    HepLorentzVector p_cms;
    for (std::vector<Particle *>::const_iterator j = final.begin();
	 j != final.end(); j++){
      if ((**j).mdstCharged()
	  && (**j).mdstCharged().get_ID() == (*i).get_ID()) {
	p_cms = (**j).p();
	sig = true;
      }
    }
    if (sig){
      // for signal (use_finalstate_for_sig == 1)
      p_cms.boost(cmboost);
      p_cms_sigB.push_back(p_cms.vect());
      Q_sigB.push_back((int)(*i).charge());
      p_cms_missB -= p_cms;
      m_et[1] += p_cms.perp();
    } else {
      HepPoint3D p((*i).trk().mhyp(2).pivot_x(),
		   (*i).trk().mhyp(2).pivot_y(),
		   (*i).trk().mhyp(2).pivot_z());
      a[0] = (*i).trk().mhyp(2).helix(0);
      a[1] = (*i).trk().mhyp(2).helix(1);
      a[2] = (*i).trk().mhyp(2).helix(2);
      a[3] = (*i).trk().mhyp(2).helix(3);
      a[4] = (*i).trk().mhyp(2).helix(4);
      Ea[0][0] = (*i).trk().mhyp(2).error(0);
      Ea[1][0] = (*i).trk().mhyp(2).error(1);
      Ea[1][1] = (*i).trk().mhyp(2).error(2);
      Ea[2][0] = (*i).trk().mhyp(2).error(3);
      Ea[2][1] = (*i).trk().mhyp(2).error(4);
      Ea[2][2] = (*i).trk().mhyp(2).error(5);
      Ea[3][0] = (*i).trk().mhyp(2).error(6);
      Ea[3][1] = (*i).trk().mhyp(2).error(7);
      Ea[3][2] = (*i).trk().mhyp(2).error(8);
      Ea[3][3] = (*i).trk().mhyp(2).error(9);
      Ea[4][0] = (*i).trk().mhyp(2).error(10);
      Ea[4][1] = (*i).trk().mhyp(2).error(11);
      Ea[4][2] = (*i).trk().mhyp(2).error(12);
      Ea[4][3] = (*i).trk().mhyp(2).error(13);
      Ea[4][4] = (*i).trk().mhyp(2).error(14);
      Helix hel(p, a, Ea);
      hel.pivot(IpProfile::position());
      if (std::fabs(hel.dr()) > 5.0 || std::fabs(hel.dz()) > 10.0) continue;

      HepSymMatrix Err(7, 0);
      HepPoint3D Pos;
      HepLorentzVector Mom(hel.momentum(0, PIMASS, Pos, Err));

      kvertexfitter kvf;
      kvf.initialVertex(IpProfile::position());
      kvf.beamProfile(IpProfile::position_err_b_life_smeared());
      kvf.addTrack(Mom, Pos, Err, (*i).charge(), PIMASS);
      
      p_cms = kvf.fit() ? Mom : kvf.momentum(0);
      //p_cms = Mom;
      p_cms.boost(cmboost);
      if (p_cms.rho() > P_MAX) continue;
      p_cms_other.push_back(p_cms.vect());
      Q_other.push_back((int)(*i).charge());
      p_cms_missA -= p_cms;
      p_cms_missB -= p_cms;
      m_et[0] += p_cms.perp();
      m_et[1] += p_cms.perp();
    }
  }

  //gammas
  for (std::vector<Mdst_gamma>::const_iterator i = gMgr.begin();
       i != gMgr.end(); i++){
    bool sig(false);
    HepLorentzVector p_cms;
    for (std::vector<Particle *>::const_iterator j = final.begin();
	 j != final.end(); j++){
      if ((**j).mdstGamma() && (**j).mdstGamma().get_ID() == (*i).get_ID()) {
	p_cms = (**j).p();
	sig = true;
      }
    }
    if (sig){
      // for signal (use_finalstate_for_sig == 1)
      p_cms.boost(cmboost);
      p_cms_sigB.push_back(p_cms.vect());
      Q_sigB.push_back(0);
      p_cms_missB -= p_cms;
      m_et[1] += p_cms.perp();
    } else {
      p_cms.setPx((*i).px());
      p_cms.setPy((*i).py());
      p_cms.setPz((*i).pz());
      p_cms.setE(p_cms.rho());
      if (p_cms.rho() < 0.05) continue;
      //if ((*i).ecl().match() || p_cms.rho() < 0.1) continue;
      p_cms.boost(cmboost);
      if (p_cms.rho() > P_MAX) continue;
      p_cms_other.push_back(p_cms.vect());
      Q_other.push_back(0);
      p_cms_missA -= p_cms;
      p_cms_missB -= p_cms;
      m_et[0] += p_cms.perp();
      m_et[1] += p_cms.perp();
    }
  }

  m_mm2[0] = p_cms_missA.e() > 0 
    ? p_cms_missA.mag2() 
    : -p_cms_missA.e()*p_cms_missA.e() - p_cms_missA.vect().mag2();
  m_mm2[1] = p_cms_missB.e() > 0 
    ? p_cms_missB.mag2() 
    : -p_cms_missB.e()*p_cms_missB.e() - p_cms_missB.vect().mag2();

  //========================
  // calculate discriminants
  //========================
  std::vector<Hep3Vector>::iterator pi, pj;
  std::vector<int>::iterator Qi, Qj;

  // calculate Hso components
  for (int i = 0; i < 3; i++){
    for (int k = 0; k < 5; k++){
      m_Hso[0][i][k] = m_Hso[1][i][k] = 0;
    }
  }

  // signal A (use_finalstate_for_sig == 0)
  for (pi = p_cms_sigA.begin(), Qi = Q_sigA.begin();
       pi != p_cms_sigA.end(); pi++, Qi++){
    const double pi_mag((*pi).mag());
    for (pj = p_cms_other.begin(), Qj = Q_other.begin();
	 pj != p_cms_other.end(); pj++, Qj++){
      const double pj_mag((*pj).mag());
      const double ij_cos((*pi)*(*pj)/pi_mag/pj_mag);
      const int c_or_n(0 == (*Qj) ? 1 : 0);  // 0: charged 1: neutral
      for (int k = 0; k < 5; k++){
	m_Hso[0][c_or_n][k] += ( k % 2 ) 
	  ? (*Qi)*(*Qj)*pj_mag*legendre(ij_cos, k)
	  : pj_mag*legendre(ij_cos, k);
      }
    }
    const double p_miss_mag(p_cms_missA.rho());
    const double i_miss_cos((*pi)*p_cms_missA.vect()/pi_mag/p_miss_mag);
    for (int k = 0; k < 5; k++){
      m_Hso[0][2][k] += ( k % 2 ) ? 0 : p_miss_mag*legendre(i_miss_cos, k);
    }
  }

  // signal B (use_finalstate_for_sig == 1)
  for (pi = p_cms_sigB.begin(), Qi = Q_sigB.begin();
       pi != p_cms_sigB.end(); pi++, Qi++){
    const double pi_mag((*pi).mag());
    for (pj = p_cms_other.begin(), Qj = Q_other.begin();
	 pj != p_cms_other.end(); pj++, Qj++){
      const double pj_mag((*pj).mag());
      const double ij_cos((*pi)*(*pj)/pi_mag/pj_mag);
      const int c_or_n(0 == (*Qj) ? 1 : 0);  // 0: charged 1: neutral
      for (int k = 0; k < 5; k++){
	m_Hso[1][c_or_n][k] += ( k % 2 ) 
	  ? (*Qi)*(*Qj)*pj_mag*legendre(ij_cos, k)
	  : pj_mag*legendre(ij_cos, k);
      }
    }
    const double p_miss_mag(p_cms_missB.rho());
    const double i_miss_cos((*pi)*p_cms_missB.vect()/pi_mag/p_miss_mag);
    for (int k = 0; k < 5; k++){
      m_Hso[1][2][k] += ( k % 2 ) ? 0 : p_miss_mag*legendre(i_miss_cos, k);
    }
  }

  // add missing to the lists
  std::vector<Hep3Vector> p_cms_otherA(p_cms_other), p_cms_otherB(p_cms_other);
  p_cms_otherA.push_back(p_cms_missA.vect());
  p_cms_otherB.push_back(p_cms_missB.vect());
  Q_other.push_back(0);

  // calculate Hoo components
  for (int k = 0; k < 5; k++){
    m_Hoo[0][k] = m_Hoo[1][k] = 0;
  }
  for (pi = p_cms_otherA.begin(), Qi = Q_other.begin();
       pi != p_cms_otherA.end(); pi++, Qi++){
    const double pi_mag((*pi).mag());
    for (pj = p_cms_otherA.begin(), Qj = Q_other.begin();
	 pj != pi; pj++, Qj++){
      const double pj_mag((*pj).mag());
      const double ij_cos((*pi)*(*pj)/pi_mag/pj_mag);
      for (int k = 0; k < 5; k++){
	m_Hoo[0][k] += ( k % 2 ) 
	  ? (*Qi)*(*Qj)*pi_mag*pj_mag*legendre(ij_cos, k)
	  : pi_mag*pj_mag*legendre(ij_cos, k);
      }
    }
  }
  for (pi = p_cms_otherB.begin(), Qi = Q_other.begin();
       pi != p_cms_otherB.end(); pi++, Qi++){
    const double pi_mag((*pi).mag());
    for (pj = p_cms_otherB.begin(), Qj = Q_other.begin();
	 pj != pi; pj++, Qj++){
      const double pj_mag((*pj).mag());
      const double ij_cos((*pi)*(*pj)/pi_mag/pj_mag);
      for (int k = 0; k < 5; k++){
	m_Hoo[1][k] += ( k % 2 ) 
	  ? (*Qi)*(*Qj)*pi_mag*pj_mag*legendre(ij_cos, k)
	  : pi_mag*pj_mag*legendre(ij_cos, k);
      }
    }
  }

  // nomalize so that it does not dependent on delta_e
  for (int k = 0; k < 5; k++){
    for (int j = 0; j < ((k%2) ? 1 : 3); j++){
      m_Hso[0][j][k] /= Hso0_max;
      m_Hso[1][j][k] /= Hso0_max;
    }
    m_Hoo[0][k] /= (Hso0_max*Hso0_max);
    m_Hoo[1][k] /= (Hso0_max*Hso0_max);
  }

  //printf("rooksfw::rooksfw: mm2=%f et=%f hoo2=%f hso02=%f\n",
  //	 m_mm2[0], m_et[0], m_Hoo[0][2], m_Hso[0][0][2]);
}
#if defined(BELLE_NAMESPACE)
}
#endif
                                                                                                                                                                                                       ksinfo.cc                                                                                           0000644 0047266 0051066 00000001335 12770400276 012364  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "ksinfo.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

//////////////
//    Ks    //
//////////////
KsUserInfo::KsUserInfo() :
  m_dr(0),
  m_dz(0),
  m_dphi(0.),
  m_fl(0.),
  m_m_raw(0.)
{
}

KsUserInfo::~KsUserInfo()
{
}

KsUserInfo::KsUserInfo(const KsUserInfo &x)
  : m_dr(x.m_dr),
    m_dz(x.m_dz),
    m_dphi(x.m_dphi),
    m_fl(x.m_fl),
    m_m_raw(x.m_m_raw)
{
}

KsUserInfo* KsUserInfo::clone(void) const
{
  KsUserInfo *x = new KsUserInfo( *this );
  return x;
}

KsUserInfo & KsUserInfo::operator = (const KsUserInfo &x)
{
  m_dr = x.m_dr;
  m_dz = x.m_dz;
  m_dphi = x.m_dphi;
  m_fl = x.m_fl;
  m_m_raw = x.m_m_raw;
  return *this;
}


#if defined(BELLE_NAMESPACE)
}
#endif 
                                                                                                                                                                                                                                                                                                   lreco_dict.cc                                                                                       0000644 0047266 0051066 00001004471 13066360656 013215  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                //
// File generated by rootcint at Tue Mar 28 13:08:14 2017

// Do NOT change. Changes will be lost next time file is generated
//

#define R__DICTIONARY_FILENAME lreco_dict
#include "RConfig.h" //rootcint 4834
#if !defined(R__ACCESS_IN_SYMBOL)
//Break the privacy of classes -- Disabled for the moment
#define private public
#define protected public
#endif

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;
#include "lreco_dict.h"

#include "TCollectionProxyInfo.h"
#include "TClass.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"

// Direct notice to TROOT of the dictionary's loading.
namespace {
   static struct DictInit {
      DictInit() {
         ROOT::RegisterModule();
      }
   } __TheDictionaryInitializer;
}

// START OF SHADOWS

namespace ROOTShadow {
   namespace Shadow {
   } // of namespace Shadow
} // of namespace ROOTShadow
// END OF SHADOWS

namespace ROOTDict {
   void DalitzVars_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_DalitzVars(void *p = 0);
   static void *newArray_DalitzVars(Long_t size, void *p);
   static void delete_DalitzVars(void *p);
   static void deleteArray_DalitzVars(void *p);
   static void destruct_DalitzVars(void *p);
   static void streamer_DalitzVars(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::DalitzVars*)
   {
      ::DalitzVars *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DalitzVars >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DalitzVars", ::DalitzVars::Class_Version(), "./DalitzVars.h", 6,
                  typeid(::DalitzVars), ::ROOT::DefineBehavior(ptr, ptr),
                  &::DalitzVars::Dictionary, isa_proxy, 0,
                  sizeof(::DalitzVars) );
      instance.SetNew(&new_DalitzVars);
      instance.SetNewArray(&newArray_DalitzVars);
      instance.SetDelete(&delete_DalitzVars);
      instance.SetDeleteArray(&deleteArray_DalitzVars);
      instance.SetDestructor(&destruct_DalitzVars);
      instance.SetStreamerFunc(&streamer_DalitzVars);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::DalitzVars*)
   {
      return GenerateInitInstanceLocal((::DalitzVars*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DalitzVars*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void DeltaEMbc_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_DeltaEMbc(void *p = 0);
   static void *newArray_DeltaEMbc(Long_t size, void *p);
   static void delete_DeltaEMbc(void *p);
   static void deleteArray_DeltaEMbc(void *p);
   static void destruct_DeltaEMbc(void *p);
   static void streamer_DeltaEMbc(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::DeltaEMbc*)
   {
      ::DeltaEMbc *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::DeltaEMbc >(0);
      static ::ROOT::TGenericClassInfo 
         instance("DeltaEMbc", ::DeltaEMbc::Class_Version(), "./DeltaEMbc.h", 6,
                  typeid(::DeltaEMbc), ::ROOT::DefineBehavior(ptr, ptr),
                  &::DeltaEMbc::Dictionary, isa_proxy, 0,
                  sizeof(::DeltaEMbc) );
      instance.SetNew(&new_DeltaEMbc);
      instance.SetNewArray(&newArray_DeltaEMbc);
      instance.SetDelete(&delete_DeltaEMbc);
      instance.SetDeleteArray(&deleteArray_DeltaEMbc);
      instance.SetDestructor(&destruct_DeltaEMbc);
      instance.SetStreamerFunc(&streamer_DeltaEMbc);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::DeltaEMbc*)
   {
      return GenerateInitInstanceLocal((::DeltaEMbc*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::DeltaEMbc*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void EvtInfo_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_EvtInfo(void *p = 0);
   static void *newArray_EvtInfo(Long_t size, void *p);
   static void delete_EvtInfo(void *p);
   static void deleteArray_EvtInfo(void *p);
   static void destruct_EvtInfo(void *p);
   static void streamer_EvtInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::EvtInfo*)
   {
      ::EvtInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EvtInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("EvtInfo", ::EvtInfo::Class_Version(), "./EvtInfo.h", 6,
                  typeid(::EvtInfo), ::ROOT::DefineBehavior(ptr, ptr),
                  &::EvtInfo::Dictionary, isa_proxy, 0,
                  sizeof(::EvtInfo) );
      instance.SetNew(&new_EvtInfo);
      instance.SetNewArray(&newArray_EvtInfo);
      instance.SetDelete(&delete_EvtInfo);
      instance.SetDeleteArray(&deleteArray_EvtInfo);
      instance.SetDestructor(&destruct_EvtInfo);
      instance.SetStreamerFunc(&streamer_EvtInfo);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::EvtInfo*)
   {
      return GenerateInitInstanceLocal((::EvtInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EvtInfo*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void SFWMInfo_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_SFWMInfo(void *p = 0);
   static void *newArray_SFWMInfo(Long_t size, void *p);
   static void delete_SFWMInfo(void *p);
   static void deleteArray_SFWMInfo(void *p);
   static void destruct_SFWMInfo(void *p);
   static void streamer_SFWMInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::SFWMInfo*)
   {
      ::SFWMInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::SFWMInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("SFWMInfo", ::SFWMInfo::Class_Version(), "./SFWMInfo.h", 6,
                  typeid(::SFWMInfo), ::ROOT::DefineBehavior(ptr, ptr),
                  &::SFWMInfo::Dictionary, isa_proxy, 0,
                  sizeof(::SFWMInfo) );
      instance.SetNew(&new_SFWMInfo);
      instance.SetNewArray(&newArray_SFWMInfo);
      instance.SetDelete(&delete_SFWMInfo);
      instance.SetDeleteArray(&deleteArray_SFWMInfo);
      instance.SetDestructor(&destruct_SFWMInfo);
      instance.SetStreamerFunc(&streamer_SFWMInfo);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::SFWMInfo*)
   {
      return GenerateInitInstanceLocal((::SFWMInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::SFWMInfo*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void EvtShape_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_EvtShape(void *p = 0);
   static void *newArray_EvtShape(Long_t size, void *p);
   static void delete_EvtShape(void *p);
   static void deleteArray_EvtShape(void *p);
   static void destruct_EvtShape(void *p);
   static void streamer_EvtShape(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::EvtShape*)
   {
      ::EvtShape *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::EvtShape >(0);
      static ::ROOT::TGenericClassInfo 
         instance("EvtShape", ::EvtShape::Class_Version(), "./EvtShape.h", 7,
                  typeid(::EvtShape), ::ROOT::DefineBehavior(ptr, ptr),
                  &::EvtShape::Dictionary, isa_proxy, 0,
                  sizeof(::EvtShape) );
      instance.SetNew(&new_EvtShape);
      instance.SetNewArray(&newArray_EvtShape);
      instance.SetDelete(&delete_EvtShape);
      instance.SetDeleteArray(&deleteArray_EvtShape);
      instance.SetDestructor(&destruct_EvtShape);
      instance.SetStreamerFunc(&streamer_EvtShape);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::EvtShape*)
   {
      return GenerateInitInstanceLocal((::EvtShape*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::EvtShape*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void GammaInfo_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_GammaInfo(void *p = 0);
   static void *newArray_GammaInfo(Long_t size, void *p);
   static void delete_GammaInfo(void *p);
   static void deleteArray_GammaInfo(void *p);
   static void destruct_GammaInfo(void *p);
   static void streamer_GammaInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::GammaInfo*)
   {
      ::GammaInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GammaInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("GammaInfo", ::GammaInfo::Class_Version(), "./GammaInfo.h", 6,
                  typeid(::GammaInfo), ::ROOT::DefineBehavior(ptr, ptr),
                  &::GammaInfo::Dictionary, isa_proxy, 0,
                  sizeof(::GammaInfo) );
      instance.SetNew(&new_GammaInfo);
      instance.SetNewArray(&newArray_GammaInfo);
      instance.SetDelete(&delete_GammaInfo);
      instance.SetDeleteArray(&deleteArray_GammaInfo);
      instance.SetDestructor(&destruct_GammaInfo);
      instance.SetStreamerFunc(&streamer_GammaInfo);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::GammaInfo*)
   {
      return GenerateInitInstanceLocal((::GammaInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::GammaInfo*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void GenHepEvt_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_GenHepEvt(void *p = 0);
   static void *newArray_GenHepEvt(Long_t size, void *p);
   static void delete_GenHepEvt(void *p);
   static void deleteArray_GenHepEvt(void *p);
   static void destruct_GenHepEvt(void *p);
   static void streamer_GenHepEvt(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::GenHepEvt*)
   {
      ::GenHepEvt *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GenHepEvt >(0);
      static ::ROOT::TGenericClassInfo 
         instance("GenHepEvt", ::GenHepEvt::Class_Version(), "./GenHepEvt.h", 7,
                  typeid(::GenHepEvt), ::ROOT::DefineBehavior(ptr, ptr),
                  &::GenHepEvt::Dictionary, isa_proxy, 0,
                  sizeof(::GenHepEvt) );
      instance.SetNew(&new_GenHepEvt);
      instance.SetNewArray(&newArray_GenHepEvt);
      instance.SetDelete(&delete_GenHepEvt);
      instance.SetDeleteArray(&deleteArray_GenHepEvt);
      instance.SetDestructor(&destruct_GenHepEvt);
      instance.SetStreamerFunc(&streamer_GenHepEvt);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::GenHepEvt*)
   {
      return GenerateInitInstanceLocal((::GenHepEvt*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::GenHepEvt*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void GenParticleInfo_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_GenParticleInfo(void *p = 0);
   static void *newArray_GenParticleInfo(Long_t size, void *p);
   static void delete_GenParticleInfo(void *p);
   static void deleteArray_GenParticleInfo(void *p);
   static void destruct_GenParticleInfo(void *p);
   static void streamer_GenParticleInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::GenParticleInfo*)
   {
      ::GenParticleInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GenParticleInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("GenParticleInfo", ::GenParticleInfo::Class_Version(), "./GenParticleInfo.h", 6,
                  typeid(::GenParticleInfo), ::ROOT::DefineBehavior(ptr, ptr),
                  &::GenParticleInfo::Dictionary, isa_proxy, 0,
                  sizeof(::GenParticleInfo) );
      instance.SetNew(&new_GenParticleInfo);
      instance.SetNewArray(&newArray_GenParticleInfo);
      instance.SetDelete(&delete_GenParticleInfo);
      instance.SetDeleteArray(&deleteArray_GenParticleInfo);
      instance.SetDestructor(&destruct_GenParticleInfo);
      instance.SetStreamerFunc(&streamer_GenParticleInfo);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::GenParticleInfo*)
   {
      return GenerateInitInstanceLocal((::GenParticleInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::GenParticleInfo*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void HamletTag_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_HamletTag(void *p = 0);
   static void *newArray_HamletTag(Long_t size, void *p);
   static void delete_HamletTag(void *p);
   static void deleteArray_HamletTag(void *p);
   static void destruct_HamletTag(void *p);
   static void streamer_HamletTag(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::HamletTag*)
   {
      ::HamletTag *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::HamletTag >(0);
      static ::ROOT::TGenericClassInfo 
         instance("HamletTag", ::HamletTag::Class_Version(), "./HamletTag.h", 6,
                  typeid(::HamletTag), ::ROOT::DefineBehavior(ptr, ptr),
                  &::HamletTag::Dictionary, isa_proxy, 0,
                  sizeof(::HamletTag) );
      instance.SetNew(&new_HamletTag);
      instance.SetNewArray(&newArray_HamletTag);
      instance.SetDelete(&delete_HamletTag);
      instance.SetDeleteArray(&deleteArray_HamletTag);
      instance.SetDestructor(&destruct_HamletTag);
      instance.SetStreamerFunc(&streamer_HamletTag);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::HamletTag*)
   {
      return GenerateInitInstanceLocal((::HamletTag*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::HamletTag*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void IPBoost_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_IPBoost(void *p = 0);
   static void *newArray_IPBoost(Long_t size, void *p);
   static void delete_IPBoost(void *p);
   static void deleteArray_IPBoost(void *p);
   static void destruct_IPBoost(void *p);
   static void streamer_IPBoost(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::IPBoost*)
   {
      ::IPBoost *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::IPBoost >(0);
      static ::ROOT::TGenericClassInfo 
         instance("IPBoost", ::IPBoost::Class_Version(), "./IPBoost.h", 6,
                  typeid(::IPBoost), ::ROOT::DefineBehavior(ptr, ptr),
                  &::IPBoost::Dictionary, isa_proxy, 0,
                  sizeof(::IPBoost) );
      instance.SetNew(&new_IPBoost);
      instance.SetNewArray(&newArray_IPBoost);
      instance.SetDelete(&delete_IPBoost);
      instance.SetDeleteArray(&deleteArray_IPBoost);
      instance.SetDestructor(&destruct_IPBoost);
      instance.SetStreamerFunc(&streamer_IPBoost);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::IPBoost*)
   {
      return GenerateInitInstanceLocal((::IPBoost*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::IPBoost*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void Ks0Info_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_Ks0Info(void *p = 0);
   static void *newArray_Ks0Info(Long_t size, void *p);
   static void delete_Ks0Info(void *p);
   static void deleteArray_Ks0Info(void *p);
   static void destruct_Ks0Info(void *p);
   static void streamer_Ks0Info(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::Ks0Info*)
   {
      ::Ks0Info *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Ks0Info >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Ks0Info", ::Ks0Info::Class_Version(), "./Ks0Info.h", 6,
                  typeid(::Ks0Info), ::ROOT::DefineBehavior(ptr, ptr),
                  &::Ks0Info::Dictionary, isa_proxy, 0,
                  sizeof(::Ks0Info) );
      instance.SetNew(&new_Ks0Info);
      instance.SetNewArray(&newArray_Ks0Info);
      instance.SetDelete(&delete_Ks0Info);
      instance.SetDeleteArray(&deleteArray_Ks0Info);
      instance.SetDestructor(&destruct_Ks0Info);
      instance.SetStreamerFunc(&streamer_Ks0Info);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::Ks0Info*)
   {
      return GenerateInitInstanceLocal((::Ks0Info*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Ks0Info*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void Pi0Info_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_Pi0Info(void *p = 0);
   static void *newArray_Pi0Info(Long_t size, void *p);
   static void delete_Pi0Info(void *p);
   static void deleteArray_Pi0Info(void *p);
   static void destruct_Pi0Info(void *p);
   static void streamer_Pi0Info(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::Pi0Info*)
   {
      ::Pi0Info *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Pi0Info >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Pi0Info", ::Pi0Info::Class_Version(), "./Pi0Info.h", 6,
                  typeid(::Pi0Info), ::ROOT::DefineBehavior(ptr, ptr),
                  &::Pi0Info::Dictionary, isa_proxy, 0,
                  sizeof(::Pi0Info) );
      instance.SetNew(&new_Pi0Info);
      instance.SetNewArray(&newArray_Pi0Info);
      instance.SetDelete(&delete_Pi0Info);
      instance.SetDeleteArray(&deleteArray_Pi0Info);
      instance.SetDestructor(&destruct_Pi0Info);
      instance.SetStreamerFunc(&streamer_Pi0Info);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::Pi0Info*)
   {
      return GenerateInitInstanceLocal((::Pi0Info*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Pi0Info*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void TrackInfo_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_TrackInfo(void *p = 0);
   static void *newArray_TrackInfo(Long_t size, void *p);
   static void delete_TrackInfo(void *p);
   static void deleteArray_TrackInfo(void *p);
   static void destruct_TrackInfo(void *p);
   static void streamer_TrackInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::TrackInfo*)
   {
      ::TrackInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TrackInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TrackInfo", ::TrackInfo::Class_Version(), "./TrackInfo.h", 6,
                  typeid(::TrackInfo), ::ROOT::DefineBehavior(ptr, ptr),
                  &::TrackInfo::Dictionary, isa_proxy, 0,
                  sizeof(::TrackInfo) );
      instance.SetNew(&new_TrackInfo);
      instance.SetNewArray(&newArray_TrackInfo);
      instance.SetDelete(&delete_TrackInfo);
      instance.SetDeleteArray(&deleteArray_TrackInfo);
      instance.SetDestructor(&destruct_TrackInfo);
      instance.SetStreamerFunc(&streamer_TrackInfo);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::TrackInfo*)
   {
      return GenerateInitInstanceLocal((::TrackInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TrackInfo*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void VertexInfo_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_VertexInfo(void *p = 0);
   static void *newArray_VertexInfo(Long_t size, void *p);
   static void delete_VertexInfo(void *p);
   static void deleteArray_VertexInfo(void *p);
   static void destruct_VertexInfo(void *p);
   static void streamer_VertexInfo(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::VertexInfo*)
   {
      ::VertexInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::VertexInfo >(0);
      static ::ROOT::TGenericClassInfo 
         instance("VertexInfo", ::VertexInfo::Class_Version(), "./VertexInfo.h", 6,
                  typeid(::VertexInfo), ::ROOT::DefineBehavior(ptr, ptr),
                  &::VertexInfo::Dictionary, isa_proxy, 0,
                  sizeof(::VertexInfo) );
      instance.SetNew(&new_VertexInfo);
      instance.SetNewArray(&newArray_VertexInfo);
      instance.SetDelete(&delete_VertexInfo);
      instance.SetDeleteArray(&deleteArray_VertexInfo);
      instance.SetDestructor(&destruct_VertexInfo);
      instance.SetStreamerFunc(&streamer_VertexInfo);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::VertexInfo*)
   {
      return GenerateInitInstanceLocal((::VertexInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::VertexInfo*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void TrackInfo2_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_TrackInfo2(void *p = 0);
   static void *newArray_TrackInfo2(Long_t size, void *p);
   static void delete_TrackInfo2(void *p);
   static void deleteArray_TrackInfo2(void *p);
   static void destruct_TrackInfo2(void *p);
   static void streamer_TrackInfo2(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::TrackInfo2*)
   {
      ::TrackInfo2 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TrackInfo2 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TrackInfo2", ::TrackInfo2::Class_Version(), "./TrackInfo2.h", 9,
                  typeid(::TrackInfo2), ::ROOT::DefineBehavior(ptr, ptr),
                  &::TrackInfo2::Dictionary, isa_proxy, 0,
                  sizeof(::TrackInfo2) );
      instance.SetNew(&new_TrackInfo2);
      instance.SetNewArray(&newArray_TrackInfo2);
      instance.SetDelete(&delete_TrackInfo2);
      instance.SetDeleteArray(&deleteArray_TrackInfo2);
      instance.SetDestructor(&destruct_TrackInfo2);
      instance.SetStreamerFunc(&streamer_TrackInfo2);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::TrackInfo2*)
   {
      return GenerateInitInstanceLocal((::TrackInfo2*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TrackInfo2*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

namespace ROOTDict {
   void H0Info_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_H0Info(void *p = 0);
   static void *newArray_H0Info(Long_t size, void *p);
   static void delete_H0Info(void *p);
   static void deleteArray_H0Info(void *p);
   static void destruct_H0Info(void *p);
   static void streamer_H0Info(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const ::H0Info*)
   {
      ::H0Info *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::H0Info >(0);
      static ::ROOT::TGenericClassInfo 
         instance("H0Info", ::H0Info::Class_Version(), "./H0Info.h", 9,
                  typeid(::H0Info), ::ROOT::DefineBehavior(ptr, ptr),
                  &::H0Info::Dictionary, isa_proxy, 0,
                  sizeof(::H0Info) );
      instance.SetNew(&new_H0Info);
      instance.SetNewArray(&newArray_H0Info);
      instance.SetDelete(&delete_H0Info);
      instance.SetDeleteArray(&deleteArray_H0Info);
      instance.SetDestructor(&destruct_H0Info);
      instance.SetStreamerFunc(&streamer_H0Info);
      return &instance;
   }
   ROOT::TGenericClassInfo *GenerateInitInstance(const ::H0Info*)
   {
      return GenerateInitInstanceLocal((::H0Info*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::H0Info*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOTDict

//______________________________________________________________________________
atomic_TClass_ptr DalitzVars::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DalitzVars::Class_Name()
{
   return "DalitzVars";
}

//______________________________________________________________________________
const char *DalitzVars::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::DalitzVars*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DalitzVars::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::DalitzVars*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void DalitzVars::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::DalitzVars*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *DalitzVars::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::DalitzVars*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr DeltaEMbc::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *DeltaEMbc::Class_Name()
{
   return "DeltaEMbc";
}

//______________________________________________________________________________
const char *DeltaEMbc::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::DeltaEMbc*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int DeltaEMbc::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::DeltaEMbc*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void DeltaEMbc::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::DeltaEMbc*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *DeltaEMbc::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::DeltaEMbc*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr EvtInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *EvtInfo::Class_Name()
{
   return "EvtInfo";
}

//______________________________________________________________________________
const char *EvtInfo::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::EvtInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int EvtInfo::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::EvtInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void EvtInfo::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::EvtInfo*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *EvtInfo::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::EvtInfo*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr SFWMInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *SFWMInfo::Class_Name()
{
   return "SFWMInfo";
}

//______________________________________________________________________________
const char *SFWMInfo::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::SFWMInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int SFWMInfo::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::SFWMInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void SFWMInfo::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::SFWMInfo*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *SFWMInfo::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::SFWMInfo*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr EvtShape::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *EvtShape::Class_Name()
{
   return "EvtShape";
}

//______________________________________________________________________________
const char *EvtShape::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::EvtShape*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int EvtShape::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::EvtShape*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void EvtShape::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::EvtShape*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *EvtShape::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::EvtShape*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr GammaInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *GammaInfo::Class_Name()
{
   return "GammaInfo";
}

//______________________________________________________________________________
const char *GammaInfo::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::GammaInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int GammaInfo::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::GammaInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void GammaInfo::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::GammaInfo*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *GammaInfo::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::GammaInfo*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr GenHepEvt::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *GenHepEvt::Class_Name()
{
   return "GenHepEvt";
}

//______________________________________________________________________________
const char *GenHepEvt::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::GenHepEvt*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int GenHepEvt::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::GenHepEvt*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void GenHepEvt::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::GenHepEvt*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *GenHepEvt::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::GenHepEvt*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr GenParticleInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *GenParticleInfo::Class_Name()
{
   return "GenParticleInfo";
}

//______________________________________________________________________________
const char *GenParticleInfo::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::GenParticleInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int GenParticleInfo::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::GenParticleInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void GenParticleInfo::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::GenParticleInfo*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *GenParticleInfo::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::GenParticleInfo*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr HamletTag::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *HamletTag::Class_Name()
{
   return "HamletTag";
}

//______________________________________________________________________________
const char *HamletTag::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::HamletTag*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int HamletTag::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::HamletTag*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void HamletTag::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::HamletTag*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *HamletTag::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::HamletTag*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr IPBoost::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *IPBoost::Class_Name()
{
   return "IPBoost";
}

//______________________________________________________________________________
const char *IPBoost::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::IPBoost*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int IPBoost::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::IPBoost*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void IPBoost::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::IPBoost*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *IPBoost::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::IPBoost*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Ks0Info::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Ks0Info::Class_Name()
{
   return "Ks0Info";
}

//______________________________________________________________________________
const char *Ks0Info::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::Ks0Info*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Ks0Info::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::Ks0Info*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void Ks0Info::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::Ks0Info*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *Ks0Info::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::Ks0Info*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr Pi0Info::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Pi0Info::Class_Name()
{
   return "Pi0Info";
}

//______________________________________________________________________________
const char *Pi0Info::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::Pi0Info*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Pi0Info::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::Pi0Info*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void Pi0Info::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::Pi0Info*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *Pi0Info::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::Pi0Info*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TrackInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TrackInfo::Class_Name()
{
   return "TrackInfo";
}

//______________________________________________________________________________
const char *TrackInfo::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::TrackInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TrackInfo::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::TrackInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void TrackInfo::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::TrackInfo*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *TrackInfo::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::TrackInfo*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr VertexInfo::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *VertexInfo::Class_Name()
{
   return "VertexInfo";
}

//______________________________________________________________________________
const char *VertexInfo::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::VertexInfo*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int VertexInfo::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::VertexInfo*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void VertexInfo::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::VertexInfo*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *VertexInfo::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::VertexInfo*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr TrackInfo2::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TrackInfo2::Class_Name()
{
   return "TrackInfo2";
}

//______________________________________________________________________________
const char *TrackInfo2::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::TrackInfo2*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TrackInfo2::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::TrackInfo2*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void TrackInfo2::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::TrackInfo2*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *TrackInfo2::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::TrackInfo2*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr H0Info::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *H0Info::Class_Name()
{
   return "H0Info";
}

//______________________________________________________________________________
const char *H0Info::ImplFileName()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::H0Info*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int H0Info::ImplFileLine()
{
   return ::ROOTDict::GenerateInitInstanceLocal((const ::H0Info*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void H0Info::Dictionary()
{
   fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::H0Info*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *H0Info::Class()
{
   if (!fgIsA) { R__LOCKGUARD2(gCINTMutex); if(!fgIsA) {fgIsA = ::ROOTDict::GenerateInitInstanceLocal((const ::H0Info*)0x0)->GetClass();} }
   return fgIsA;
}

//______________________________________________________________________________
void DalitzVars::Streamer(TBuffer &R__b)
{
   // Stream an object of class DalitzVars.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.ReadStaticArray((double*)msq);
      R__b.ReadStaticArray((double*)cosh);
      R__b.CheckByteCount(R__s, R__c, DalitzVars::IsA());
   } else {
      R__c = R__b.WriteVersion(DalitzVars::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.WriteArray(msq, 3);
      R__b.WriteArray(cosh, 3);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void DalitzVars::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class DalitzVars.
      TClass *R__cl = ::DalitzVars::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "msq[3]", msq);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "cosh[3]", cosh);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_DalitzVars(void *p) {
      return  p ? new(p) ::DalitzVars : new ::DalitzVars;
   }
   static void *newArray_DalitzVars(Long_t nElements, void *p) {
      return p ? new(p) ::DalitzVars[nElements] : new ::DalitzVars[nElements];
   }
   // Wrapper around operator delete
   static void delete_DalitzVars(void *p) {
      delete ((::DalitzVars*)p);
   }
   static void deleteArray_DalitzVars(void *p) {
      delete [] ((::DalitzVars*)p);
   }
   static void destruct_DalitzVars(void *p) {
      typedef ::DalitzVars current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DalitzVars(TBuffer &buf, void *obj) {
      ((::DalitzVars*)obj)->::DalitzVars::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::DalitzVars

//______________________________________________________________________________
void DeltaEMbc::Streamer(TBuffer &R__b)
{
   // Stream an object of class DeltaEMbc.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> de;
      R__b >> mbc;
      R__b.CheckByteCount(R__s, R__c, DeltaEMbc::IsA());
   } else {
      R__c = R__b.WriteVersion(DeltaEMbc::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << de;
      R__b << mbc;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void DeltaEMbc::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class DeltaEMbc.
      TClass *R__cl = ::DeltaEMbc::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "de", &de);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "mbc", &mbc);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_DeltaEMbc(void *p) {
      return  p ? new(p) ::DeltaEMbc : new ::DeltaEMbc;
   }
   static void *newArray_DeltaEMbc(Long_t nElements, void *p) {
      return p ? new(p) ::DeltaEMbc[nElements] : new ::DeltaEMbc[nElements];
   }
   // Wrapper around operator delete
   static void delete_DeltaEMbc(void *p) {
      delete ((::DeltaEMbc*)p);
   }
   static void deleteArray_DeltaEMbc(void *p) {
      delete [] ((::DeltaEMbc*)p);
   }
   static void destruct_DeltaEMbc(void *p) {
      typedef ::DeltaEMbc current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_DeltaEMbc(TBuffer &buf, void *obj) {
      ((::DeltaEMbc*)obj)->::DeltaEMbc::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::DeltaEMbc

//______________________________________________________________________________
void EvtInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class EvtInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> exp;
      R__b >> run;
      R__b >> evtn;
      R__b.CheckByteCount(R__s, R__c, EvtInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(EvtInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << exp;
      R__b << run;
      R__b << evtn;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void EvtInfo::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class EvtInfo.
      TClass *R__cl = ::EvtInfo::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "exp", &exp);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "run", &run);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "evtn", &evtn);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_EvtInfo(void *p) {
      return  p ? new(p) ::EvtInfo : new ::EvtInfo;
   }
   static void *newArray_EvtInfo(Long_t nElements, void *p) {
      return p ? new(p) ::EvtInfo[nElements] : new ::EvtInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_EvtInfo(void *p) {
      delete ((::EvtInfo*)p);
   }
   static void deleteArray_EvtInfo(void *p) {
      delete [] ((::EvtInfo*)p);
   }
   static void destruct_EvtInfo(void *p) {
      typedef ::EvtInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_EvtInfo(TBuffer &buf, void *obj) {
      ((::EvtInfo*)obj)->::EvtInfo::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::EvtInfo

//______________________________________________________________________________
void EvtShape::Streamer(TBuffer &R__b)
{
   // Stream an object of class EvtShape.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> cos_thr;
      R__b >> thr_sig;
      R__b >> thr_oth;
      k0.Streamer(R__b);
      k1.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, EvtShape::IsA());
   } else {
      R__c = R__b.WriteVersion(EvtShape::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << cos_thr;
      R__b << thr_sig;
      R__b << thr_oth;
      k0.Streamer(R__b);
      k1.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void EvtShape::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class EvtShape.
      TClass *R__cl = ::EvtShape::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "cos_thr", &cos_thr);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "thr_sig", &thr_sig);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "thr_oth", &thr_oth);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "k0", &k0);
      R__insp.InspectMember(k0, "k0.");
      R__insp.Inspect(R__cl, R__insp.GetParent(), "k1", &k1);
      R__insp.InspectMember(k1, "k1.");
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_EvtShape(void *p) {
      return  p ? new(p) ::EvtShape : new ::EvtShape;
   }
   static void *newArray_EvtShape(Long_t nElements, void *p) {
      return p ? new(p) ::EvtShape[nElements] : new ::EvtShape[nElements];
   }
   // Wrapper around operator delete
   static void delete_EvtShape(void *p) {
      delete ((::EvtShape*)p);
   }
   static void deleteArray_EvtShape(void *p) {
      delete [] ((::EvtShape*)p);
   }
   static void destruct_EvtShape(void *p) {
      typedef ::EvtShape current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_EvtShape(TBuffer &buf, void *obj) {
      ((::EvtShape*)obj)->::EvtShape::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::EvtShape

//______________________________________________________________________________
void GammaInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class GammaInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.ReadStaticArray((double*)p);
      R__b.CheckByteCount(R__s, R__c, GammaInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(GammaInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.WriteArray(p, 3);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void GammaInfo::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class GammaInfo.
      TClass *R__cl = ::GammaInfo::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "p[3]", p);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_GammaInfo(void *p) {
      return  p ? new(p) ::GammaInfo : new ::GammaInfo;
   }
   static void *newArray_GammaInfo(Long_t nElements, void *p) {
      return p ? new(p) ::GammaInfo[nElements] : new ::GammaInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_GammaInfo(void *p) {
      delete ((::GammaInfo*)p);
   }
   static void deleteArray_GammaInfo(void *p) {
      delete [] ((::GammaInfo*)p);
   }
   static void destruct_GammaInfo(void *p) {
      typedef ::GammaInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_GammaInfo(TBuffer &buf, void *obj) {
      ((::GammaInfo*)obj)->::GammaInfo::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::GammaInfo

//______________________________________________________________________________
void GenHepEvt::Streamer(TBuffer &R__b)
{
   // Stream an object of class GenHepEvt.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      {
         vector<int> &R__stl =  idhep;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<int> &R__stl =  daF;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<int> &R__stl =  daL;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, GenHepEvt::IsA());
   } else {
      R__c = R__b.WriteVersion(GenHepEvt::IsA(), kTRUE);
      TObject::Streamer(R__b);
      {
         vector<int> &R__stl =  idhep;
         int R__n=(true) ? int(R__stl.size()) : 0;
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<int> &R__stl =  daF;
         int R__n=(true) ? int(R__stl.size()) : 0;
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<int> &R__stl =  daL;
         int R__n=(true) ? int(R__stl.size()) : 0;
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void GenHepEvt::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class GenHepEvt.
      TClass *R__cl = ::GenHepEvt::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "idhep", (void*)&idhep);
      R__insp.InspectMember("vector<int>", (void*)&idhep, "idhep.", false);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "daF", (void*)&daF);
      R__insp.InspectMember("vector<int>", (void*)&daF, "daF.", false);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "daL", (void*)&daL);
      R__insp.InspectMember("vector<int>", (void*)&daL, "daL.", false);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_GenHepEvt(void *p) {
      return  p ? new(p) ::GenHepEvt : new ::GenHepEvt;
   }
   static void *newArray_GenHepEvt(Long_t nElements, void *p) {
      return p ? new(p) ::GenHepEvt[nElements] : new ::GenHepEvt[nElements];
   }
   // Wrapper around operator delete
   static void delete_GenHepEvt(void *p) {
      delete ((::GenHepEvt*)p);
   }
   static void deleteArray_GenHepEvt(void *p) {
      delete [] ((::GenHepEvt*)p);
   }
   static void destruct_GenHepEvt(void *p) {
      typedef ::GenHepEvt current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_GenHepEvt(TBuffer &buf, void *obj) {
      ((::GenHepEvt*)obj)->::GenHepEvt::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::GenHepEvt

//______________________________________________________________________________
void GenParticleInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class GenParticleInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> id;
      R__b >> flag;
      R__b.ReadStaticArray((int*)chain);
      R__b.CheckByteCount(R__s, R__c, GenParticleInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(GenParticleInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << id;
      R__b << flag;
      R__b.WriteArray(chain, 9);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void GenParticleInfo::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class GenParticleInfo.
      TClass *R__cl = ::GenParticleInfo::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "id", &id);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "flag", &flag);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "chain[9]", chain);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_GenParticleInfo(void *p) {
      return  p ? new(p) ::GenParticleInfo : new ::GenParticleInfo;
   }
   static void *newArray_GenParticleInfo(Long_t nElements, void *p) {
      return p ? new(p) ::GenParticleInfo[nElements] : new ::GenParticleInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_GenParticleInfo(void *p) {
      delete ((::GenParticleInfo*)p);
   }
   static void deleteArray_GenParticleInfo(void *p) {
      delete [] ((::GenParticleInfo*)p);
   }
   static void destruct_GenParticleInfo(void *p) {
      typedef ::GenParticleInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_GenParticleInfo(TBuffer &buf, void *obj) {
      ((::GenParticleInfo*)obj)->::GenParticleInfo::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::GenParticleInfo

//______________________________________________________________________________
void HamletTag::Streamer(TBuffer &R__b)
{
   // Stream an object of class HamletTag.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> tag_LH;
      R__b >> tag_LH_err;
      R__b >> tagl;
      R__b >> taglid;
      R__b >> flv;
      R__b.CheckByteCount(R__s, R__c, HamletTag::IsA());
   } else {
      R__c = R__b.WriteVersion(HamletTag::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << tag_LH;
      R__b << tag_LH_err;
      R__b << tagl;
      R__b << taglid;
      R__b << flv;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void HamletTag::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class HamletTag.
      TClass *R__cl = ::HamletTag::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "tag_LH", &tag_LH);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "tag_LH_err", &tag_LH_err);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "tagl", &tagl);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "taglid", &taglid);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "flv", &flv);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_HamletTag(void *p) {
      return  p ? new(p) ::HamletTag : new ::HamletTag;
   }
   static void *newArray_HamletTag(Long_t nElements, void *p) {
      return p ? new(p) ::HamletTag[nElements] : new ::HamletTag[nElements];
   }
   // Wrapper around operator delete
   static void delete_HamletTag(void *p) {
      delete ((::HamletTag*)p);
   }
   static void deleteArray_HamletTag(void *p) {
      delete [] ((::HamletTag*)p);
   }
   static void destruct_HamletTag(void *p) {
      typedef ::HamletTag current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_HamletTag(TBuffer &buf, void *obj) {
      ((::HamletTag*)obj)->::HamletTag::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::HamletTag

//______________________________________________________________________________
void IPBoost::Streamer(TBuffer &R__b)
{
   // Stream an object of class IPBoost.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.ReadStaticArray((double*)ip);
      R__b.ReadStaticArray((double*)boost);
      R__b.CheckByteCount(R__s, R__c, IPBoost::IsA());
   } else {
      R__c = R__b.WriteVersion(IPBoost::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.WriteArray(ip, 3);
      R__b.WriteArray(boost, 3);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void IPBoost::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class IPBoost.
      TClass *R__cl = ::IPBoost::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "ip[3]", ip);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "boost[3]", boost);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_IPBoost(void *p) {
      return  p ? new(p) ::IPBoost : new ::IPBoost;
   }
   static void *newArray_IPBoost(Long_t nElements, void *p) {
      return p ? new(p) ::IPBoost[nElements] : new ::IPBoost[nElements];
   }
   // Wrapper around operator delete
   static void delete_IPBoost(void *p) {
      delete ((::IPBoost*)p);
   }
   static void deleteArray_IPBoost(void *p) {
      delete [] ((::IPBoost*)p);
   }
   static void destruct_IPBoost(void *p) {
      typedef ::IPBoost current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_IPBoost(TBuffer &buf, void *obj) {
      ((::IPBoost*)obj)->::IPBoost::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::IPBoost

//______________________________________________________________________________
void Ks0Info::Streamer(TBuffer &R__b)
{
   // Stream an object of class Ks0Info.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> m;
      R__b >> m_raw;
      R__b.ReadStaticArray((double*)p);
      R__b >> dr;
      R__b >> dz;
      R__b >> dphi;
      R__b >> fl;
      R__b.CheckByteCount(R__s, R__c, Ks0Info::IsA());
   } else {
      R__c = R__b.WriteVersion(Ks0Info::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << m;
      R__b << m_raw;
      R__b.WriteArray(p, 3);
      R__b << dr;
      R__b << dz;
      R__b << dphi;
      R__b << fl;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void Ks0Info::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class Ks0Info.
      TClass *R__cl = ::Ks0Info::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "m", &m);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "m_raw", &m_raw);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "p[3]", p);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "dr", &dr);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "dz", &dz);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "dphi", &dphi);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "fl", &fl);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_Ks0Info(void *p) {
      return  p ? new(p) ::Ks0Info : new ::Ks0Info;
   }
   static void *newArray_Ks0Info(Long_t nElements, void *p) {
      return p ? new(p) ::Ks0Info[nElements] : new ::Ks0Info[nElements];
   }
   // Wrapper around operator delete
   static void delete_Ks0Info(void *p) {
      delete ((::Ks0Info*)p);
   }
   static void deleteArray_Ks0Info(void *p) {
      delete [] ((::Ks0Info*)p);
   }
   static void destruct_Ks0Info(void *p) {
      typedef ::Ks0Info current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Ks0Info(TBuffer &buf, void *obj) {
      ((::Ks0Info*)obj)->::Ks0Info::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::Ks0Info

//______________________________________________________________________________
void Pi0Info::Streamer(TBuffer &R__b)
{
   // Stream an object of class Pi0Info.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> m;
      R__b >> m_raw;
      R__b.ReadStaticArray((double*)p);
      R__b >> eg1;
      R__b >> eg2;
      R__b >> chisq;
      R__b.CheckByteCount(R__s, R__c, Pi0Info::IsA());
   } else {
      R__c = R__b.WriteVersion(Pi0Info::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << m;
      R__b << m_raw;
      R__b.WriteArray(p, 3);
      R__b << eg1;
      R__b << eg2;
      R__b << chisq;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void Pi0Info::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class Pi0Info.
      TClass *R__cl = ::Pi0Info::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "m", &m);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "m_raw", &m_raw);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "p[3]", p);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "eg1", &eg1);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "eg2", &eg2);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "chisq", &chisq);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_Pi0Info(void *p) {
      return  p ? new(p) ::Pi0Info : new ::Pi0Info;
   }
   static void *newArray_Pi0Info(Long_t nElements, void *p) {
      return p ? new(p) ::Pi0Info[nElements] : new ::Pi0Info[nElements];
   }
   // Wrapper around operator delete
   static void delete_Pi0Info(void *p) {
      delete ((::Pi0Info*)p);
   }
   static void deleteArray_Pi0Info(void *p) {
      delete [] ((::Pi0Info*)p);
   }
   static void destruct_Pi0Info(void *p) {
      typedef ::Pi0Info current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_Pi0Info(TBuffer &buf, void *obj) {
      ((::Pi0Info*)obj)->::Pi0Info::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::Pi0Info

//______________________________________________________________________________
void SFWMInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class SFWMInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> kmm2;
      R__b >> ket;
      R__b >> khso00;
      R__b >> khso01;
      R__b >> khso02;
      R__b >> khso03;
      R__b >> khso04;
      R__b >> khso10;
      R__b >> khso12;
      R__b >> khso14;
      R__b >> khso20;
      R__b >> khso22;
      R__b >> khso24;
      R__b >> khoo0;
      R__b >> khoo1;
      R__b >> khoo2;
      R__b >> khoo3;
      R__b >> khoo4;
      R__b.CheckByteCount(R__s, R__c, SFWMInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(SFWMInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << kmm2;
      R__b << ket;
      R__b << khso00;
      R__b << khso01;
      R__b << khso02;
      R__b << khso03;
      R__b << khso04;
      R__b << khso10;
      R__b << khso12;
      R__b << khso14;
      R__b << khso20;
      R__b << khso22;
      R__b << khso24;
      R__b << khoo0;
      R__b << khoo1;
      R__b << khoo2;
      R__b << khoo3;
      R__b << khoo4;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void SFWMInfo::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class SFWMInfo.
      TClass *R__cl = ::SFWMInfo::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "kmm2", &kmm2);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "ket", &ket);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khso00", &khso00);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khso01", &khso01);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khso02", &khso02);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khso03", &khso03);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khso04", &khso04);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khso10", &khso10);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khso12", &khso12);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khso14", &khso14);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khso20", &khso20);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khso22", &khso22);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khso24", &khso24);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khoo0", &khoo0);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khoo1", &khoo1);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khoo2", &khoo2);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khoo3", &khoo3);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "khoo4", &khoo4);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_SFWMInfo(void *p) {
      return  p ? new(p) ::SFWMInfo : new ::SFWMInfo;
   }
   static void *newArray_SFWMInfo(Long_t nElements, void *p) {
      return p ? new(p) ::SFWMInfo[nElements] : new ::SFWMInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_SFWMInfo(void *p) {
      delete ((::SFWMInfo*)p);
   }
   static void deleteArray_SFWMInfo(void *p) {
      delete [] ((::SFWMInfo*)p);
   }
   static void destruct_SFWMInfo(void *p) {
      typedef ::SFWMInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_SFWMInfo(TBuffer &buf, void *obj) {
      ((::SFWMInfo*)obj)->::SFWMInfo::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::SFWMInfo

//______________________________________________________________________________
void TrackInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class TrackInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b.ReadStaticArray((double*)p);
      R__b >> r;
      R__b >> z;
      R__b >> rz_svd;
      R__b >> rphi_svd;
      R__b >> atckpi;
      R__b >> atckp;
      R__b >> eid;
      R__b >> muid;
      R__b.CheckByteCount(R__s, R__c, TrackInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(TrackInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b.WriteArray(p, 3);
      R__b << r;
      R__b << z;
      R__b << rz_svd;
      R__b << rphi_svd;
      R__b << atckpi;
      R__b << atckp;
      R__b << eid;
      R__b << muid;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void TrackInfo::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class TrackInfo.
      TClass *R__cl = ::TrackInfo::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "p[3]", p);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "r", &r);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "z", &z);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "rz_svd", &rz_svd);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "rphi_svd", &rphi_svd);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "atckpi", &atckpi);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "atckp", &atckp);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "eid", &eid);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "muid", &muid);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_TrackInfo(void *p) {
      return  p ? new(p) ::TrackInfo : new ::TrackInfo;
   }
   static void *newArray_TrackInfo(Long_t nElements, void *p) {
      return p ? new(p) ::TrackInfo[nElements] : new ::TrackInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_TrackInfo(void *p) {
      delete ((::TrackInfo*)p);
   }
   static void deleteArray_TrackInfo(void *p) {
      delete [] ((::TrackInfo*)p);
   }
   static void destruct_TrackInfo(void *p) {
      typedef ::TrackInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TrackInfo(TBuffer &buf, void *obj) {
      ((::TrackInfo*)obj)->::TrackInfo::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::TrackInfo

//______________________________________________________________________________
void VertexInfo::Streamer(TBuffer &R__b)
{
   // Stream an object of class VertexInfo.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> z;
      R__b >> sz;
      R__b >> ntrk;
      R__b >> ndf;
      R__b >> chisq;
      R__b >> cl;
      R__b.CheckByteCount(R__s, R__c, VertexInfo::IsA());
   } else {
      R__c = R__b.WriteVersion(VertexInfo::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << z;
      R__b << sz;
      R__b << ntrk;
      R__b << ndf;
      R__b << chisq;
      R__b << cl;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void VertexInfo::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class VertexInfo.
      TClass *R__cl = ::VertexInfo::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "z", &z);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "sz", &sz);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "ntrk", &ntrk);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "ndf", &ndf);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "chisq", &chisq);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "cl", &cl);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_VertexInfo(void *p) {
      return  p ? new(p) ::VertexInfo : new ::VertexInfo;
   }
   static void *newArray_VertexInfo(Long_t nElements, void *p) {
      return p ? new(p) ::VertexInfo[nElements] : new ::VertexInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_VertexInfo(void *p) {
      delete ((::VertexInfo*)p);
   }
   static void deleteArray_VertexInfo(void *p) {
      delete [] ((::VertexInfo*)p);
   }
   static void destruct_VertexInfo(void *p) {
      typedef ::VertexInfo current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_VertexInfo(TBuffer &buf, void *obj) {
      ((::VertexInfo*)obj)->::VertexInfo::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::VertexInfo

//______________________________________________________________________________
void TrackInfo2::Streamer(TBuffer &R__b)
{
   // Stream an object of class TrackInfo2.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> p;
      R__b >> costh;
      R__b >> phi;
      R__b >> r;
      R__b >> z;
      R__b >> rz_svd;
      R__b >> rphi_svd;
      R__b >> atckpi;
      R__b >> atckp;
      R__b >> eid;
      R__b >> muid;
      R__b.CheckByteCount(R__s, R__c, TrackInfo2::IsA());
   } else {
      R__c = R__b.WriteVersion(TrackInfo2::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << p;
      R__b << costh;
      R__b << phi;
      R__b << r;
      R__b << z;
      R__b << rz_svd;
      R__b << rphi_svd;
      R__b << atckpi;
      R__b << atckp;
      R__b << eid;
      R__b << muid;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void TrackInfo2::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class TrackInfo2.
      TClass *R__cl = ::TrackInfo2::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "p", &p);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "costh", &costh);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "phi", &phi);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "r", &r);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "z", &z);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "rz_svd", &rz_svd);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "rphi_svd", &rphi_svd);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "atckpi", &atckpi);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "atckp", &atckp);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "eid", &eid);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "muid", &muid);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_TrackInfo2(void *p) {
      return  p ? new(p) ::TrackInfo2 : new ::TrackInfo2;
   }
   static void *newArray_TrackInfo2(Long_t nElements, void *p) {
      return p ? new(p) ::TrackInfo2[nElements] : new ::TrackInfo2[nElements];
   }
   // Wrapper around operator delete
   static void delete_TrackInfo2(void *p) {
      delete ((::TrackInfo2*)p);
   }
   static void deleteArray_TrackInfo2(void *p) {
      delete [] ((::TrackInfo2*)p);
   }
   static void destruct_TrackInfo2(void *p) {
      typedef ::TrackInfo2 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TrackInfo2(TBuffer &buf, void *obj) {
      ((::TrackInfo2*)obj)->::TrackInfo2::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::TrackInfo2

//______________________________________________________________________________
void H0Info::Streamer(TBuffer &R__b)
{
   // Stream an object of class H0Info.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> m;
      R__b >> p;
      R__b >> costh;
      R__b >> phi;
      R__b >> mchi2;
      R__b.CheckByteCount(R__s, R__c, H0Info::IsA());
   } else {
      R__c = R__b.WriteVersion(H0Info::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << m;
      R__b << p;
      R__b << costh;
      R__b << phi;
      R__b << mchi2;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

//______________________________________________________________________________
void H0Info::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class H0Info.
      TClass *R__cl = ::H0Info::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "m", &m);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "p", &p);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "costh", &costh);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "phi", &phi);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "mchi2", &mchi2);
      TObject::ShowMembers(R__insp);
}

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_H0Info(void *p) {
      return  p ? new(p) ::H0Info : new ::H0Info;
   }
   static void *newArray_H0Info(Long_t nElements, void *p) {
      return p ? new(p) ::H0Info[nElements] : new ::H0Info[nElements];
   }
   // Wrapper around operator delete
   static void delete_H0Info(void *p) {
      delete ((::H0Info*)p);
   }
   static void deleteArray_H0Info(void *p) {
      delete [] ((::H0Info*)p);
   }
   static void destruct_H0Info(void *p) {
      typedef ::H0Info current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_H0Info(TBuffer &buf, void *obj) {
      ((::H0Info*)obj)->::H0Info::Streamer(buf);
   }
} // end of namespace ROOTDict for class ::H0Info

namespace ROOTDict {
   void vectorlEintgR_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void vectorlEintgR_Dictionary();
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static ROOT::TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>),0);
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "/sw/belle/cern/root_v5.34.36/cint/cint/lib/prec_stl/vector", 49,
                  typeid(vector<int>), ::ROOT::DefineBehavior(ptr, ptr),
                  0, &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo( ::ROOT::TCollectionProxyInfo::Generate( ::ROOT::TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static void vectorlEintgR_Dictionary() {
      ::ROOTDict::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
   }

} // end of namespace ROOTDict

namespace ROOTDict {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOTDict for class vector<int>

/********************************************************
* lreco_dict.cc
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************/

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

#if defined(__GNUC__) && __GNUC__ >= 4 && ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

extern "C" void G__cpp_reset_tagtablelreco_dict();

extern "C" void G__set_cpp_environmentlreco_dict() {
  G__cpp_reset_tagtablelreco_dict();
}
#include <new>
extern "C" int G__cpp_dllrevlreco_dict() { return(30051515); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* DalitzVars */
static int G__lreco_dict_168_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   DalitzVars* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new DalitzVars[n];
     } else {
       p = new((void*) gvp) DalitzVars[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new DalitzVars;
     } else {
       p = new((void*) gvp) DalitzVars;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_DalitzVars));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_168_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((DalitzVars*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_168_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) DalitzVars::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_168_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) DalitzVars::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_168_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) DalitzVars::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_168_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      DalitzVars::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_168_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((DalitzVars*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_168_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) DalitzVars::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_168_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) DalitzVars::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_168_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) DalitzVars::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_168_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) DalitzVars::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_168_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   DalitzVars* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new DalitzVars(*(DalitzVars*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_DalitzVars));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef DalitzVars G__TDalitzVars;
static int G__lreco_dict_168_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (DalitzVars*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((DalitzVars*) (soff+(sizeof(DalitzVars)*i)))->~G__TDalitzVars();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (DalitzVars*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((DalitzVars*) (soff))->~G__TDalitzVars();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_168_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   DalitzVars* dest = (DalitzVars*) G__getstructoffset();
   *dest = *(DalitzVars*) libp->para[0].ref;
   const DalitzVars& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* DeltaEMbc */
static int G__lreco_dict_169_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   DeltaEMbc* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new DeltaEMbc[n];
     } else {
       p = new((void*) gvp) DeltaEMbc[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new DeltaEMbc;
     } else {
       p = new((void*) gvp) DeltaEMbc;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_DeltaEMbc));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_169_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((DeltaEMbc*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_169_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) DeltaEMbc::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_169_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) DeltaEMbc::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_169_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) DeltaEMbc::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_169_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      DeltaEMbc::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_169_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((DeltaEMbc*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_169_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) DeltaEMbc::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_169_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) DeltaEMbc::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_169_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) DeltaEMbc::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_169_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) DeltaEMbc::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_169_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   DeltaEMbc* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new DeltaEMbc(*(DeltaEMbc*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_DeltaEMbc));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef DeltaEMbc G__TDeltaEMbc;
static int G__lreco_dict_169_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (DeltaEMbc*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((DeltaEMbc*) (soff+(sizeof(DeltaEMbc)*i)))->~G__TDeltaEMbc();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (DeltaEMbc*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((DeltaEMbc*) (soff))->~G__TDeltaEMbc();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_169_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   DeltaEMbc* dest = (DeltaEMbc*) G__getstructoffset();
   *dest = *(DeltaEMbc*) libp->para[0].ref;
   const DeltaEMbc& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* EvtInfo */
static int G__lreco_dict_170_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   EvtInfo* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new EvtInfo[n];
     } else {
       p = new((void*) gvp) EvtInfo[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new EvtInfo;
     } else {
       p = new((void*) gvp) EvtInfo;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_EvtInfo));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_170_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((EvtInfo*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_170_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) ((const EvtInfo*) G__getstructoffset())->SVD());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_170_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) EvtInfo::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_170_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) EvtInfo::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_170_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) EvtInfo::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_170_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      EvtInfo::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_170_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((EvtInfo*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_170_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) EvtInfo::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_170_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) EvtInfo::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_170_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) EvtInfo::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_170_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) EvtInfo::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_170_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   EvtInfo* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new EvtInfo(*(EvtInfo*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_EvtInfo));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef EvtInfo G__TEvtInfo;
static int G__lreco_dict_170_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (EvtInfo*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((EvtInfo*) (soff+(sizeof(EvtInfo)*i)))->~G__TEvtInfo();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (EvtInfo*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((EvtInfo*) (soff))->~G__TEvtInfo();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_170_0_18(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   EvtInfo* dest = (EvtInfo*) G__getstructoffset();
   *dest = *(EvtInfo*) libp->para[0].ref;
   const EvtInfo& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* SFWMInfo */
static int G__lreco_dict_171_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   SFWMInfo* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new SFWMInfo[n];
     } else {
       p = new((void*) gvp) SFWMInfo[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new SFWMInfo;
     } else {
       p = new((void*) gvp) SFWMInfo;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_SFWMInfo));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_171_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((SFWMInfo*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_171_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) SFWMInfo::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_171_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) SFWMInfo::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_171_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) SFWMInfo::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_171_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      SFWMInfo::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_171_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((SFWMInfo*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_171_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) SFWMInfo::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_171_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) SFWMInfo::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_171_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) SFWMInfo::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_171_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) SFWMInfo::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_171_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   SFWMInfo* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new SFWMInfo(*(SFWMInfo*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_SFWMInfo));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef SFWMInfo G__TSFWMInfo;
static int G__lreco_dict_171_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (SFWMInfo*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((SFWMInfo*) (soff+(sizeof(SFWMInfo)*i)))->~G__TSFWMInfo();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (SFWMInfo*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((SFWMInfo*) (soff))->~G__TSFWMInfo();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_171_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   SFWMInfo* dest = (SFWMInfo*) G__getstructoffset();
   *dest = *(SFWMInfo*) libp->para[0].ref;
   const SFWMInfo& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* EvtShape */
static int G__lreco_dict_172_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   EvtShape* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new EvtShape[n];
     } else {
       p = new((void*) gvp) EvtShape[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new EvtShape;
     } else {
       p = new((void*) gvp) EvtShape;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_EvtShape));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_172_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((EvtShape*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_172_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) EvtShape::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_172_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) EvtShape::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_172_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) EvtShape::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_172_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      EvtShape::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_172_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((EvtShape*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_172_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) EvtShape::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_172_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) EvtShape::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_172_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) EvtShape::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_172_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) EvtShape::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_172_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   EvtShape* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new EvtShape(*(EvtShape*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_EvtShape));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef EvtShape G__TEvtShape;
static int G__lreco_dict_172_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (EvtShape*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((EvtShape*) (soff+(sizeof(EvtShape)*i)))->~G__TEvtShape();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (EvtShape*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((EvtShape*) (soff))->~G__TEvtShape();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_172_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   EvtShape* dest = (EvtShape*) G__getstructoffset();
   *dest = *(EvtShape*) libp->para[0].ref;
   const EvtShape& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* GammaInfo */
static int G__lreco_dict_173_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   GammaInfo* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new GammaInfo[n];
     } else {
       p = new((void*) gvp) GammaInfo[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new GammaInfo;
     } else {
       p = new((void*) gvp) GammaInfo;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_GammaInfo));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((GammaInfo*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const GammaInfo*) G__getstructoffset())->P());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const GammaInfo*) G__getstructoffset())->pt());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const GammaInfo*) G__getstructoffset())->costh());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) GammaInfo::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) GammaInfo::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) GammaInfo::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_9(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      GammaInfo::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((GammaInfo*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) GammaInfo::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) GammaInfo::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) GammaInfo::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_173_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) GammaInfo::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_173_0_18(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   GammaInfo* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new GammaInfo(*(GammaInfo*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_GammaInfo));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef GammaInfo G__TGammaInfo;
static int G__lreco_dict_173_0_19(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (GammaInfo*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((GammaInfo*) (soff+(sizeof(GammaInfo)*i)))->~G__TGammaInfo();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (GammaInfo*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((GammaInfo*) (soff))->~G__TGammaInfo();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_173_0_20(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   GammaInfo* dest = (GammaInfo*) G__getstructoffset();
   *dest = *(GammaInfo*) libp->para[0].ref;
   const GammaInfo& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* GenHepEvt */
static int G__lreco_dict_174_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   GenHepEvt* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new GenHepEvt[n];
     } else {
       p = new((void*) gvp) GenHepEvt[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new GenHepEvt;
     } else {
       p = new((void*) gvp) GenHepEvt;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_GenHepEvt));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_174_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((GenHepEvt*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_174_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) GenHepEvt::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_174_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) GenHepEvt::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_174_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) GenHepEvt::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_174_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      GenHepEvt::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_174_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((GenHepEvt*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_174_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) GenHepEvt::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_174_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) GenHepEvt::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_174_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) GenHepEvt::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_174_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) GenHepEvt::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_174_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   GenHepEvt* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new GenHepEvt(*(GenHepEvt*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_GenHepEvt));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef GenHepEvt G__TGenHepEvt;
static int G__lreco_dict_174_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (GenHepEvt*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((GenHepEvt*) (soff+(sizeof(GenHepEvt)*i)))->~G__TGenHepEvt();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (GenHepEvt*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((GenHepEvt*) (soff))->~G__TGenHepEvt();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_174_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   GenHepEvt* dest = (GenHepEvt*) G__getstructoffset();
   *dest = *(GenHepEvt*) libp->para[0].ref;
   const GenHepEvt& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* GenParticleInfo */
static int G__lreco_dict_179_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   GenParticleInfo* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new GenParticleInfo[n];
     } else {
       p = new((void*) gvp) GenParticleInfo[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new GenParticleInfo;
     } else {
       p = new((void*) gvp) GenParticleInfo;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_GenParticleInfo));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_179_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((GenParticleInfo*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_179_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) GenParticleInfo::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_179_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) GenParticleInfo::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_179_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) GenParticleInfo::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_179_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      GenParticleInfo::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_179_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((GenParticleInfo*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_179_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) GenParticleInfo::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_179_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) GenParticleInfo::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_179_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) GenParticleInfo::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_179_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) GenParticleInfo::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_179_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   GenParticleInfo* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new GenParticleInfo(*(GenParticleInfo*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_GenParticleInfo));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef GenParticleInfo G__TGenParticleInfo;
static int G__lreco_dict_179_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (GenParticleInfo*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((GenParticleInfo*) (soff+(sizeof(GenParticleInfo)*i)))->~G__TGenParticleInfo();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (GenParticleInfo*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((GenParticleInfo*) (soff))->~G__TGenParticleInfo();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_179_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   GenParticleInfo* dest = (GenParticleInfo*) G__getstructoffset();
   *dest = *(GenParticleInfo*) libp->para[0].ref;
   const GenParticleInfo& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* HamletTag */
static int G__lreco_dict_180_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   HamletTag* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new HamletTag[n];
     } else {
       p = new((void*) gvp) HamletTag[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new HamletTag;
     } else {
       p = new((void*) gvp) HamletTag;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_HamletTag));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_180_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((HamletTag*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_180_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) HamletTag::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_180_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) HamletTag::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_180_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) HamletTag::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_180_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      HamletTag::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_180_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((HamletTag*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_180_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) HamletTag::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_180_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) HamletTag::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_180_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) HamletTag::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_180_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) HamletTag::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_180_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   HamletTag* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new HamletTag(*(HamletTag*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_HamletTag));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef HamletTag G__THamletTag;
static int G__lreco_dict_180_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (HamletTag*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((HamletTag*) (soff+(sizeof(HamletTag)*i)))->~G__THamletTag();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (HamletTag*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((HamletTag*) (soff))->~G__THamletTag();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_180_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   HamletTag* dest = (HamletTag*) G__getstructoffset();
   *dest = *(HamletTag*) libp->para[0].ref;
   const HamletTag& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* IPBoost */
static int G__lreco_dict_181_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   IPBoost* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new IPBoost[n];
     } else {
       p = new((void*) gvp) IPBoost[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new IPBoost;
     } else {
       p = new((void*) gvp) IPBoost;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_IPBoost));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_181_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((IPBoost*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_181_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) IPBoost::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_181_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) IPBoost::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_181_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) IPBoost::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_181_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      IPBoost::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_181_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((IPBoost*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_181_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) IPBoost::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_181_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) IPBoost::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_181_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) IPBoost::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_181_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) IPBoost::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_181_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   IPBoost* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new IPBoost(*(IPBoost*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_IPBoost));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef IPBoost G__TIPBoost;
static int G__lreco_dict_181_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (IPBoost*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((IPBoost*) (soff+(sizeof(IPBoost)*i)))->~G__TIPBoost();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (IPBoost*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((IPBoost*) (soff))->~G__TIPBoost();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_181_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   IPBoost* dest = (IPBoost*) G__getstructoffset();
   *dest = *(IPBoost*) libp->para[0].ref;
   const IPBoost& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Ks0Info */
static int G__lreco_dict_182_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   Ks0Info* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new Ks0Info[n];
     } else {
       p = new((void*) gvp) Ks0Info[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new Ks0Info;
     } else {
       p = new((void*) gvp) Ks0Info;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_Ks0Info));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_182_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((Ks0Info*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_182_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) Ks0Info::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_182_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) Ks0Info::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_182_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) Ks0Info::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_182_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      Ks0Info::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_182_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((Ks0Info*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_182_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) Ks0Info::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_182_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) Ks0Info::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_182_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) Ks0Info::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_182_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) Ks0Info::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_182_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   Ks0Info* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new Ks0Info(*(Ks0Info*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_Ks0Info));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef Ks0Info G__TKs0Info;
static int G__lreco_dict_182_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (Ks0Info*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((Ks0Info*) (soff+(sizeof(Ks0Info)*i)))->~G__TKs0Info();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (Ks0Info*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((Ks0Info*) (soff))->~G__TKs0Info();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_182_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   Ks0Info* dest = (Ks0Info*) G__getstructoffset();
   *dest = *(Ks0Info*) libp->para[0].ref;
   const Ks0Info& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Pi0Info */
static int G__lreco_dict_183_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   Pi0Info* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new Pi0Info[n];
     } else {
       p = new((void*) gvp) Pi0Info[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new Pi0Info;
     } else {
       p = new((void*) gvp) Pi0Info;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_Pi0Info));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((Pi0Info*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const Pi0Info*) G__getstructoffset())->asym());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const Pi0Info*) G__getstructoffset())->P());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const Pi0Info*) G__getstructoffset())->pt());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const Pi0Info*) G__getstructoffset())->costh());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const Pi0Info*) G__getstructoffset())->egmin());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const Pi0Info*) G__getstructoffset())->egmax());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_9(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) Pi0Info::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) Pi0Info::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) Pi0Info::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      Pi0Info::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((Pi0Info*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) Pi0Info::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_18(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) Pi0Info::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_19(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) Pi0Info::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_183_0_20(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) Pi0Info::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_183_0_21(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   Pi0Info* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new Pi0Info(*(Pi0Info*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_Pi0Info));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef Pi0Info G__TPi0Info;
static int G__lreco_dict_183_0_22(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (Pi0Info*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((Pi0Info*) (soff+(sizeof(Pi0Info)*i)))->~G__TPi0Info();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (Pi0Info*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((Pi0Info*) (soff))->~G__TPi0Info();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_183_0_23(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   Pi0Info* dest = (Pi0Info*) G__getstructoffset();
   *dest = *(Pi0Info*) libp->para[0].ref;
   const Pi0Info& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* TrackInfo */
static int G__lreco_dict_184_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   TrackInfo* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new TrackInfo[n];
     } else {
       p = new((void*) gvp) TrackInfo[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new TrackInfo;
     } else {
       p = new((void*) gvp) TrackInfo;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((TrackInfo*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const TrackInfo*) G__getstructoffset())->P());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const TrackInfo*) G__getstructoffset())->pt());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const TrackInfo*) G__getstructoffset())->costh());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) TrackInfo::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) TrackInfo::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) TrackInfo::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_9(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      TrackInfo::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((TrackInfo*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) TrackInfo::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) TrackInfo::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) TrackInfo::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_184_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) TrackInfo::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_184_0_18(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   TrackInfo* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new TrackInfo(*(TrackInfo*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef TrackInfo G__TTrackInfo;
static int G__lreco_dict_184_0_19(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (TrackInfo*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((TrackInfo*) (soff+(sizeof(TrackInfo)*i)))->~G__TTrackInfo();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (TrackInfo*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((TrackInfo*) (soff))->~G__TTrackInfo();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_184_0_20(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   TrackInfo* dest = (TrackInfo*) G__getstructoffset();
   *dest = *(TrackInfo*) libp->para[0].ref;
   const TrackInfo& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* VertexInfo */
static int G__lreco_dict_185_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   VertexInfo* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new VertexInfo[n];
     } else {
       p = new((void*) gvp) VertexInfo[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new VertexInfo;
     } else {
       p = new((void*) gvp) VertexInfo;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_VertexInfo));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_185_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((VertexInfo*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_185_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) VertexInfo::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_185_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) VertexInfo::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_185_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) VertexInfo::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_185_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      VertexInfo::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_185_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((VertexInfo*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_185_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) VertexInfo::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_185_0_12(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) VertexInfo::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_185_0_13(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) VertexInfo::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_185_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) VertexInfo::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_185_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   VertexInfo* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new VertexInfo(*(VertexInfo*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_VertexInfo));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef VertexInfo G__TVertexInfo;
static int G__lreco_dict_185_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (VertexInfo*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((VertexInfo*) (soff+(sizeof(VertexInfo)*i)))->~G__TVertexInfo();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (VertexInfo*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((VertexInfo*) (soff))->~G__TVertexInfo();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_185_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   VertexInfo* dest = (VertexInfo*) G__getstructoffset();
   *dest = *(VertexInfo*) libp->para[0].ref;
   const VertexInfo& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* TrackInfo2 */
static int G__lreco_dict_186_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   TrackInfo2* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new TrackInfo2[n];
     } else {
       p = new((void*) gvp) TrackInfo2[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new TrackInfo2;
     } else {
       p = new((void*) gvp) TrackInfo2;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo2));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((TrackInfo2*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const TrackInfo2*) G__getstructoffset())->px());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const TrackInfo2*) G__getstructoffset())->py());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const TrackInfo2*) G__getstructoffset())->pz());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const TrackInfo2*) G__getstructoffset())->pt());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) TrackInfo2::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) TrackInfo2::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_9(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) TrackInfo2::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      TrackInfo2::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((TrackInfo2*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) TrackInfo2::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) TrackInfo2::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) TrackInfo2::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_186_0_18(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) TrackInfo2::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_186_0_19(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   TrackInfo2* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new TrackInfo2(*(TrackInfo2*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo2));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef TrackInfo2 G__TTrackInfo2;
static int G__lreco_dict_186_0_20(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (TrackInfo2*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((TrackInfo2*) (soff+(sizeof(TrackInfo2)*i)))->~G__TTrackInfo2();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (TrackInfo2*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((TrackInfo2*) (soff))->~G__TTrackInfo2();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_186_0_21(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   TrackInfo2* dest = (TrackInfo2*) G__getstructoffset();
   *dest = *(TrackInfo2*) libp->para[0].ref;
   const TrackInfo2& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* H0Info */
static int G__lreco_dict_187_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   H0Info* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new H0Info[n];
     } else {
       p = new((void*) gvp) H0Info[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new H0Info;
     } else {
       p = new((void*) gvp) H0Info;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_H0Info));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((H0Info*) G__getstructoffset())->Clear();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_3(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const H0Info*) G__getstructoffset())->px());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const H0Info*) G__getstructoffset())->py());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const H0Info*) G__getstructoffset())->pz());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letdouble(result7, 100, (double) ((const H0Info*) G__getstructoffset())->pt());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) H0Info::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) H0Info::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_9(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) H0Info::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      H0Info::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_14(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((H0Info*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) H0Info::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) H0Info::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) H0Info::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__lreco_dict_187_0_18(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) H0Info::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__lreco_dict_187_0_19(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   H0Info* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new H0Info(*(H0Info*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__lreco_dictLN_H0Info));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef H0Info G__TH0Info;
static int G__lreco_dict_187_0_20(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (H0Info*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((H0Info*) (soff+(sizeof(H0Info)*i)))->~G__TH0Info();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (H0Info*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((H0Info*) (soff))->~G__TH0Info();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__lreco_dict_187_0_21(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   H0Info* dest = (H0Info*) G__getstructoffset();
   *dest = *(H0Info*) libp->para[0].ref;
   const H0Info& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Setting up global function */

/*********************************************************
* Member function Stub
*********************************************************/

/* DalitzVars */

/* DeltaEMbc */

/* EvtInfo */

/* SFWMInfo */

/* EvtShape */

/* GammaInfo */

/* GenHepEvt */

/* GenParticleInfo */

/* HamletTag */

/* IPBoost */

/* Ks0Info */

/* Pi0Info */

/* TrackInfo */

/* VertexInfo */

/* TrackInfo2 */

/* H0Info */

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfunclreco_dict {
 public:
  G__Sizep2memfunclreco_dict(): p(&G__Sizep2memfunclreco_dict::sizep2memfunc) {}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfunclreco_dict::*p)();
};

size_t G__get_sizep2memfunclreco_dict()
{
  G__Sizep2memfunclreco_dict a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritancelreco_dict() {

   /* Setting up class inheritance */
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_DalitzVars))) {
     DalitzVars *G__Lderived;
     G__Lderived=(DalitzVars*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_DalitzVars),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_DeltaEMbc))) {
     DeltaEMbc *G__Lderived;
     G__Lderived=(DeltaEMbc*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_DeltaEMbc),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_EvtInfo))) {
     EvtInfo *G__Lderived;
     G__Lderived=(EvtInfo*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_EvtInfo),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_SFWMInfo))) {
     SFWMInfo *G__Lderived;
     G__Lderived=(SFWMInfo*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_SFWMInfo),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_EvtShape))) {
     EvtShape *G__Lderived;
     G__Lderived=(EvtShape*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_EvtShape),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_GammaInfo))) {
     GammaInfo *G__Lderived;
     G__Lderived=(GammaInfo*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_GammaInfo),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_GenHepEvt))) {
     GenHepEvt *G__Lderived;
     G__Lderived=(GenHepEvt*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_GenHepEvt),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_GenParticleInfo))) {
     GenParticleInfo *G__Lderived;
     G__Lderived=(GenParticleInfo*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_GenParticleInfo),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_HamletTag))) {
     HamletTag *G__Lderived;
     G__Lderived=(HamletTag*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_HamletTag),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_IPBoost))) {
     IPBoost *G__Lderived;
     G__Lderived=(IPBoost*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_IPBoost),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_Ks0Info))) {
     Ks0Info *G__Lderived;
     G__Lderived=(Ks0Info*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_Ks0Info),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_Pi0Info))) {
     Pi0Info *G__Lderived;
     G__Lderived=(Pi0Info*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_Pi0Info),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo))) {
     TrackInfo *G__Lderived;
     G__Lderived=(TrackInfo*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_VertexInfo))) {
     VertexInfo *G__Lderived;
     G__Lderived=(VertexInfo*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_VertexInfo),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo2))) {
     TrackInfo2 *G__Lderived;
     G__Lderived=(TrackInfo2*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo2),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__lreco_dictLN_H0Info))) {
     H0Info *G__Lderived;
     G__Lderived=(H0Info*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__lreco_dictLN_H0Info),G__get_linked_tagnum(&G__lreco_dictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetablelreco_dict() {

   /* Setting up typedef entry */
   G__search_typename2("Version_t",115,-1,0,-1);
   G__setnewtype(-1,"Class version identifier (short)",0);
   G__search_typename2("vector<ROOT::TSchemaHelper>",117,G__get_linked_tagnum(&G__lreco_dictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__lreco_dictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__lreco_dictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__lreco_dictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__lreco_dictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<TVirtualArray*>",117,G__get_linked_tagnum(&G__lreco_dictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__lreco_dictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__lreco_dictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__lreco_dictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__lreco_dictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<int>",117,G__get_linked_tagnum(&G__lreco_dictLN_vectorlEintcOallocatorlEintgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__lreco_dictLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__lreco_dictLN_vectorlEintcOallocatorlEintgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__lreco_dictLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__lreco_dictLN_vectorlEintcOallocatorlEintgRsPgR));
   G__setnewtype(-1,NULL,0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* DalitzVars */
static void G__setup_memvarDalitzVars(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_DalitzVars));
   { DalitzVars *p; p=(DalitzVars*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->msq)-(long)(p)),100,0,0,-1,-1,-1,1,"msq[3]=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->cosh)-(long)(p)),100,0,0,-1,-1,-1,1,"cosh[3]=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* DeltaEMbc */
static void G__setup_memvarDeltaEMbc(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_DeltaEMbc));
   { DeltaEMbc *p; p=(DeltaEMbc*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->de)-(long)(p)),100,0,0,-1,-1,-1,1,"de=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->mbc)-(long)(p)),100,0,0,-1,-1,-1,1,"mbc=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* EvtInfo */
static void G__setup_memvarEvtInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_EvtInfo));
   { EvtInfo *p; p=(EvtInfo*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->exp)-(long)(p)),105,0,0,-1,-1,-1,1,"exp=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->run)-(long)(p)),105,0,0,-1,-1,-1,1,"run=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->evtn)-(long)(p)),105,0,0,-1,-1,-1,1,"evtn=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* SFWMInfo */
static void G__setup_memvarSFWMInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_SFWMInfo));
   { SFWMInfo *p; p=(SFWMInfo*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->kmm2)-(long)(p)),100,0,0,-1,-1,-1,1,"kmm2=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->ket)-(long)(p)),100,0,0,-1,-1,-1,1,"ket=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khso00)-(long)(p)),100,0,0,-1,-1,-1,1,"khso00=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khso01)-(long)(p)),100,0,0,-1,-1,-1,1,"khso01=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khso02)-(long)(p)),100,0,0,-1,-1,-1,1,"khso02=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khso03)-(long)(p)),100,0,0,-1,-1,-1,1,"khso03=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khso04)-(long)(p)),100,0,0,-1,-1,-1,1,"khso04=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khso10)-(long)(p)),100,0,0,-1,-1,-1,1,"khso10=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khso12)-(long)(p)),100,0,0,-1,-1,-1,1,"khso12=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khso14)-(long)(p)),100,0,0,-1,-1,-1,1,"khso14=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khso20)-(long)(p)),100,0,0,-1,-1,-1,1,"khso20=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khso22)-(long)(p)),100,0,0,-1,-1,-1,1,"khso22=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khso24)-(long)(p)),100,0,0,-1,-1,-1,1,"khso24=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khoo0)-(long)(p)),100,0,0,-1,-1,-1,1,"khoo0=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khoo1)-(long)(p)),100,0,0,-1,-1,-1,1,"khoo1=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khoo2)-(long)(p)),100,0,0,-1,-1,-1,1,"khoo2=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khoo3)-(long)(p)),100,0,0,-1,-1,-1,1,"khoo3=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->khoo4)-(long)(p)),100,0,0,-1,-1,-1,1,"khoo4=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* EvtShape */
static void G__setup_memvarEvtShape(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_EvtShape));
   { EvtShape *p; p=(EvtShape*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->cos_thr)-(long)(p)),100,0,0,-1,-1,-1,1,"cos_thr=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->thr_sig)-(long)(p)),100,0,0,-1,-1,-1,1,"thr_sig=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->thr_oth)-(long)(p)),100,0,0,-1,-1,-1,1,"thr_oth=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->k0)-(long)(p)),117,0,0,G__get_linked_tagnum(&G__lreco_dictLN_SFWMInfo),-1,-1,1,"k0=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->k1)-(long)(p)),117,0,0,G__get_linked_tagnum(&G__lreco_dictLN_SFWMInfo),-1,-1,1,"k1=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* GammaInfo */
static void G__setup_memvarGammaInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_GammaInfo));
   { GammaInfo *p; p=(GammaInfo*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->p)-(long)(p)),100,0,0,-1,-1,-1,1,"p[3]=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* GenHepEvt */
static void G__setup_memvarGenHepEvt(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_GenHepEvt));
   { GenHepEvt *p; p=(GenHepEvt*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->idhep)-(long)(p)),117,0,0,G__get_linked_tagnum(&G__lreco_dictLN_vectorlEintcOallocatorlEintgRsPgR),G__defined_typename("vector<int>"),-1,1,"idhep=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->daF)-(long)(p)),117,0,0,G__get_linked_tagnum(&G__lreco_dictLN_vectorlEintcOallocatorlEintgRsPgR),G__defined_typename("vector<int>"),-1,1,"daF=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->daL)-(long)(p)),117,0,0,G__get_linked_tagnum(&G__lreco_dictLN_vectorlEintcOallocatorlEintgRsPgR),G__defined_typename("vector<int>"),-1,1,"daL=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* GenParticleInfo */
static void G__setup_memvarGenParticleInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_GenParticleInfo));
   { GenParticleInfo *p; p=(GenParticleInfo*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->id)-(long)(p)),105,0,0,-1,-1,-1,1,"id=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->flag)-(long)(p)),105,0,0,-1,-1,-1,1,"flag=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->chain)-(long)(p)),105,0,0,-1,-1,-1,1,"chain[9]=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* HamletTag */
static void G__setup_memvarHamletTag(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_HamletTag));
   { HamletTag *p; p=(HamletTag*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->tag_LH)-(long)(p)),100,0,0,-1,-1,-1,1,"tag_LH=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->tag_LH_err)-(long)(p)),100,0,0,-1,-1,-1,1,"tag_LH_err=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->tagl)-(long)(p)),105,0,0,-1,-1,-1,1,"tagl=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->taglid)-(long)(p)),105,0,0,-1,-1,-1,1,"taglid=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->flv)-(long)(p)),105,0,0,-1,-1,-1,1,"flv=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* IPBoost */
static void G__setup_memvarIPBoost(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_IPBoost));
   { IPBoost *p; p=(IPBoost*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->ip)-(long)(p)),100,0,0,-1,-1,-1,1,"ip[3]=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->boost)-(long)(p)),100,0,0,-1,-1,-1,1,"boost[3]=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* Ks0Info */
static void G__setup_memvarKs0Info(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_Ks0Info));
   { Ks0Info *p; p=(Ks0Info*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->m)-(long)(p)),100,0,0,-1,-1,-1,1,"m=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->m_raw)-(long)(p)),100,0,0,-1,-1,-1,1,"m_raw=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->p)-(long)(p)),100,0,0,-1,-1,-1,1,"p[3]=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->dr)-(long)(p)),100,0,0,-1,-1,-1,1,"dr=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->dz)-(long)(p)),100,0,0,-1,-1,-1,1,"dz=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->dphi)-(long)(p)),100,0,0,-1,-1,-1,1,"dphi=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->fl)-(long)(p)),100,0,0,-1,-1,-1,1,"fl=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* Pi0Info */
static void G__setup_memvarPi0Info(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_Pi0Info));
   { Pi0Info *p; p=(Pi0Info*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->m)-(long)(p)),100,0,0,-1,-1,-1,1,"m=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->m_raw)-(long)(p)),100,0,0,-1,-1,-1,1,"m_raw=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->p)-(long)(p)),100,0,0,-1,-1,-1,1,"p[3]=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->eg1)-(long)(p)),100,0,0,-1,-1,-1,1,"eg1=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->eg2)-(long)(p)),100,0,0,-1,-1,-1,1,"eg2=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->chisq)-(long)(p)),100,0,0,-1,-1,-1,1,"chisq=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* TrackInfo */
static void G__setup_memvarTrackInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo));
   { TrackInfo *p; p=(TrackInfo*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->p)-(long)(p)),100,0,0,-1,-1,-1,1,"p[3]=",0,"Momentum");
   G__memvar_setup((void*)((long)(&p->r)-(long)(p)),100,0,0,-1,-1,-1,1,"r=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->z)-(long)(p)),100,0,0,-1,-1,-1,1,"z=",0,"Impact parameters");
   G__memvar_setup((void*)((long)(&p->rz_svd)-(long)(p)),105,0,0,-1,-1,-1,1,"rz_svd=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->rphi_svd)-(long)(p)),105,0,0,-1,-1,-1,1,"rphi_svd=",0,"SVD hits");
   G__memvar_setup((void*)((long)(&p->atckpi)-(long)(p)),100,0,0,-1,-1,-1,1,"atckpi=",0,"Identification");
   G__memvar_setup((void*)((long)(&p->atckp)-(long)(p)),100,0,0,-1,-1,-1,1,"atckp=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->eid)-(long)(p)),100,0,0,-1,-1,-1,1,"eid=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->muid)-(long)(p)),100,0,0,-1,-1,-1,1,"muid=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* VertexInfo */
static void G__setup_memvarVertexInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_VertexInfo));
   { VertexInfo *p; p=(VertexInfo*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->z)-(long)(p)),100,0,0,-1,-1,-1,1,"z=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->sz)-(long)(p)),100,0,0,-1,-1,-1,1,"sz=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->ntrk)-(long)(p)),105,0,0,-1,-1,-1,1,"ntrk=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->ndf)-(long)(p)),105,0,0,-1,-1,-1,1,"ndf=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->chisq)-(long)(p)),100,0,0,-1,-1,-1,1,"chisq=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->cl)-(long)(p)),100,0,0,-1,-1,-1,1,"cl=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* TrackInfo2 */
static void G__setup_memvarTrackInfo2(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo2));
   { TrackInfo2 *p; p=(TrackInfo2*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->p)-(long)(p)),100,0,0,-1,-1,-1,1,"p=",0,"Momentum");
   G__memvar_setup((void*)((long)(&p->costh)-(long)(p)),100,0,0,-1,-1,-1,1,"costh=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->phi)-(long)(p)),100,0,0,-1,-1,-1,1,"phi=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->r)-(long)(p)),100,0,0,-1,-1,-1,1,"r=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->z)-(long)(p)),100,0,0,-1,-1,-1,1,"z=",0,"Impact parameters");
   G__memvar_setup((void*)((long)(&p->rz_svd)-(long)(p)),105,0,0,-1,-1,-1,1,"rz_svd=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->rphi_svd)-(long)(p)),105,0,0,-1,-1,-1,1,"rphi_svd=",0,"SVD hits");
   G__memvar_setup((void*)((long)(&p->atckpi)-(long)(p)),100,0,0,-1,-1,-1,1,"atckpi=",0,"Identification");
   G__memvar_setup((void*)((long)(&p->atckp)-(long)(p)),100,0,0,-1,-1,-1,1,"atckp=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->eid)-(long)(p)),100,0,0,-1,-1,-1,1,"eid=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->muid)-(long)(p)),100,0,0,-1,-1,-1,1,"muid=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* H0Info */
static void G__setup_memvarH0Info(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__lreco_dictLN_H0Info));
   { H0Info *p; p=(H0Info*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->m)-(long)(p)),100,0,0,-1,-1,-1,1,"m=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->p)-(long)(p)),100,0,0,-1,-1,-1,1,"p=",0,"Momentum");
   G__memvar_setup((void*)((long)(&p->costh)-(long)(p)),100,0,0,-1,-1,-1,1,"costh=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->phi)-(long)(p)),100,0,0,-1,-1,-1,1,"phi=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->mchi2)-(long)(p)),100,0,0,-1,-1,-1,1,"mchi2=",0,(char*)NULL);
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__lreco_dictLN_TClass),G__defined_typename("atomic_TClass_ptr"),-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}

extern "C" void G__cpp_setup_memvarlreco_dict() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/
static void G__setup_memfuncDalitzVars(void) {
   /* DalitzVars */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_DalitzVars));
   G__memfunc_setup("DalitzVars",1028,G__lreco_dict_168_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_DalitzVars), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_168_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_168_0_3, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&DalitzVars::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_168_0_4, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&DalitzVars::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_168_0_5, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&DalitzVars::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_168_0_6, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&DalitzVars::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_168_0_10, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_168_0_11, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&DalitzVars::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_168_0_12, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&DalitzVars::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_168_0_13, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&DalitzVars::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_168_0_14, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&DalitzVars::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("DalitzVars", 1028, G__lreco_dict_168_0_15, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_DalitzVars), -1, 0, 1, 1, 1, 0, "u 'DalitzVars' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~DalitzVars", 1154, G__lreco_dict_168_0_16, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_168_0_17, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_DalitzVars), -1, 1, 1, 1, 1, 0, "u 'DalitzVars' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncDeltaEMbc(void) {
   /* DeltaEMbc */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_DeltaEMbc));
   G__memfunc_setup("DeltaEMbc",833,G__lreco_dict_169_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_DeltaEMbc), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_169_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_169_0_3, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&DeltaEMbc::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_169_0_4, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&DeltaEMbc::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_169_0_5, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&DeltaEMbc::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_169_0_6, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&DeltaEMbc::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_169_0_10, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_169_0_11, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&DeltaEMbc::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_169_0_12, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&DeltaEMbc::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_169_0_13, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&DeltaEMbc::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_169_0_14, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&DeltaEMbc::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("DeltaEMbc", 833, G__lreco_dict_169_0_15, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_DeltaEMbc), -1, 0, 1, 1, 1, 0, "u 'DeltaEMbc' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~DeltaEMbc", 959, G__lreco_dict_169_0_16, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_169_0_17, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_DeltaEMbc), -1, 1, 1, 1, 1, 0, "u 'DeltaEMbc' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncEvtInfo(void) {
   /* EvtInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_EvtInfo));
   G__memfunc_setup("EvtInfo",699,G__lreco_dict_170_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_EvtInfo), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_170_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("SVD",237,G__lreco_dict_170_0_3, 105, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_170_0_4, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&EvtInfo::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_170_0_5, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&EvtInfo::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_170_0_6, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&EvtInfo::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_170_0_7, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&EvtInfo::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_170_0_11, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_170_0_12, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&EvtInfo::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_170_0_13, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&EvtInfo::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_170_0_14, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&EvtInfo::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_170_0_15, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&EvtInfo::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("EvtInfo", 699, G__lreco_dict_170_0_16, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_EvtInfo), -1, 0, 1, 1, 1, 0, "u 'EvtInfo' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~EvtInfo", 825, G__lreco_dict_170_0_17, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_170_0_18, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_EvtInfo), -1, 1, 1, 1, 1, 0, "u 'EvtInfo' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncSFWMInfo(void) {
   /* SFWMInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_SFWMInfo));
   G__memfunc_setup("SFWMInfo",713,G__lreco_dict_171_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_SFWMInfo), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_171_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_171_0_3, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&SFWMInfo::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_171_0_4, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&SFWMInfo::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_171_0_5, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&SFWMInfo::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_171_0_6, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&SFWMInfo::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_171_0_10, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_171_0_11, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&SFWMInfo::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_171_0_12, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&SFWMInfo::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_171_0_13, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&SFWMInfo::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_171_0_14, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&SFWMInfo::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("SFWMInfo", 713, G__lreco_dict_171_0_15, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_SFWMInfo), -1, 0, 1, 1, 1, 0, "u 'SFWMInfo' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~SFWMInfo", 839, G__lreco_dict_171_0_16, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_171_0_17, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_SFWMInfo), -1, 1, 1, 1, 1, 0, "u 'SFWMInfo' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncEvtShape(void) {
   /* EvtShape */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_EvtShape));
   G__memfunc_setup("EvtShape",800,G__lreco_dict_172_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_EvtShape), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_172_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_172_0_3, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&EvtShape::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_172_0_4, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&EvtShape::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_172_0_5, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&EvtShape::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_172_0_6, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&EvtShape::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_172_0_10, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_172_0_11, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&EvtShape::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_172_0_12, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&EvtShape::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_172_0_13, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&EvtShape::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_172_0_14, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&EvtShape::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("EvtShape", 800, G__lreco_dict_172_0_15, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_EvtShape), -1, 0, 1, 1, 1, 0, "u 'EvtShape' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~EvtShape", 926, G__lreco_dict_172_0_16, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_172_0_17, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_EvtShape), -1, 1, 1, 1, 1, 0, "u 'EvtShape' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncGammaInfo(void) {
   /* GammaInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_GammaInfo));
   G__memfunc_setup("GammaInfo",879,G__lreco_dict_173_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_GammaInfo), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_173_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("P",80,G__lreco_dict_173_0_3, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("pt",228,G__lreco_dict_173_0_4, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("costh",545,G__lreco_dict_173_0_5, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_173_0_6, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&GammaInfo::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_173_0_7, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&GammaInfo::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_173_0_8, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&GammaInfo::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_173_0_9, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&GammaInfo::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_173_0_13, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_173_0_14, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&GammaInfo::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_173_0_15, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&GammaInfo::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_173_0_16, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&GammaInfo::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_173_0_17, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&GammaInfo::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("GammaInfo", 879, G__lreco_dict_173_0_18, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_GammaInfo), -1, 0, 1, 1, 1, 0, "u 'GammaInfo' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~GammaInfo", 1005, G__lreco_dict_173_0_19, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_173_0_20, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_GammaInfo), -1, 1, 1, 1, 1, 0, "u 'GammaInfo' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncGenHepEvt(void) {
   /* GenHepEvt */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_GenHepEvt));
   G__memfunc_setup("GenHepEvt",870,G__lreco_dict_174_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_GenHepEvt), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_174_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_174_0_3, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&GenHepEvt::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_174_0_4, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&GenHepEvt::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_174_0_5, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&GenHepEvt::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_174_0_6, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&GenHepEvt::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_174_0_10, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_174_0_11, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&GenHepEvt::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_174_0_12, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&GenHepEvt::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_174_0_13, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&GenHepEvt::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_174_0_14, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&GenHepEvt::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("GenHepEvt", 870, G__lreco_dict_174_0_15, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_GenHepEvt), -1, 0, 1, 1, 1, 0, "u 'GenHepEvt' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~GenHepEvt", 996, G__lreco_dict_174_0_16, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_174_0_17, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_GenHepEvt), -1, 1, 1, 1, 1, 0, "u 'GenHepEvt' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncGenParticleInfo(void) {
   /* GenParticleInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_GenParticleInfo));
   G__memfunc_setup("GenParticleInfo",1498,G__lreco_dict_179_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_GenParticleInfo), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_179_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_179_0_3, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&GenParticleInfo::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_179_0_4, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&GenParticleInfo::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_179_0_5, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&GenParticleInfo::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_179_0_6, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&GenParticleInfo::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_179_0_10, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_179_0_11, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&GenParticleInfo::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_179_0_12, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&GenParticleInfo::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_179_0_13, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&GenParticleInfo::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_179_0_14, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&GenParticleInfo::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("GenParticleInfo", 1498, G__lreco_dict_179_0_15, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_GenParticleInfo), -1, 0, 1, 1, 1, 0, "u 'GenParticleInfo' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~GenParticleInfo", 1624, G__lreco_dict_179_0_16, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_179_0_17, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_GenParticleInfo), -1, 1, 1, 1, 1, 0, "u 'GenParticleInfo' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncHamletTag(void) {
   /* HamletTag */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_HamletTag));
   G__memfunc_setup("HamletTag",887,G__lreco_dict_180_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_HamletTag), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_180_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_180_0_3, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&HamletTag::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_180_0_4, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&HamletTag::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_180_0_5, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&HamletTag::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_180_0_6, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&HamletTag::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_180_0_10, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_180_0_11, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&HamletTag::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_180_0_12, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&HamletTag::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_180_0_13, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&HamletTag::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_180_0_14, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&HamletTag::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("HamletTag", 887, G__lreco_dict_180_0_15, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_HamletTag), -1, 0, 1, 1, 1, 0, "u 'HamletTag' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~HamletTag", 1013, G__lreco_dict_180_0_16, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_180_0_17, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_HamletTag), -1, 1, 1, 1, 1, 0, "u 'HamletTag' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncIPBoost(void) {
   /* IPBoost */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_IPBoost));
   G__memfunc_setup("IPBoost",672,G__lreco_dict_181_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_IPBoost), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_181_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_181_0_3, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&IPBoost::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_181_0_4, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&IPBoost::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_181_0_5, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&IPBoost::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_181_0_6, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&IPBoost::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_181_0_10, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_181_0_11, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&IPBoost::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_181_0_12, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&IPBoost::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_181_0_13, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&IPBoost::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_181_0_14, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&IPBoost::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("IPBoost", 672, G__lreco_dict_181_0_15, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_IPBoost), -1, 0, 1, 1, 1, 0, "u 'IPBoost' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~IPBoost", 798, G__lreco_dict_181_0_16, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_181_0_17, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_IPBoost), -1, 1, 1, 1, 1, 0, "u 'IPBoost' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncKs0Info(void) {
   /* Ks0Info */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_Ks0Info));
   G__memfunc_setup("Ks0Info",634,G__lreco_dict_182_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_Ks0Info), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_182_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_182_0_3, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&Ks0Info::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_182_0_4, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&Ks0Info::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_182_0_5, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&Ks0Info::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_182_0_6, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&Ks0Info::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_182_0_10, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_182_0_11, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&Ks0Info::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_182_0_12, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&Ks0Info::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_182_0_13, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&Ks0Info::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_182_0_14, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&Ks0Info::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("Ks0Info", 634, G__lreco_dict_182_0_15, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_Ks0Info), -1, 0, 1, 1, 1, 0, "u 'Ks0Info' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~Ks0Info", 760, G__lreco_dict_182_0_16, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_182_0_17, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_Ks0Info), -1, 1, 1, 1, 1, 0, "u 'Ks0Info' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncPi0Info(void) {
   /* Pi0Info */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_Pi0Info));
   G__memfunc_setup("Pi0Info",629,G__lreco_dict_183_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_Pi0Info), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_183_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("asym",442,G__lreco_dict_183_0_3, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("P",80,G__lreco_dict_183_0_4, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("pt",228,G__lreco_dict_183_0_5, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("costh",545,G__lreco_dict_183_0_6, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("egmin",528,G__lreco_dict_183_0_7, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("egmax",530,G__lreco_dict_183_0_8, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_183_0_9, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&Pi0Info::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_183_0_10, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&Pi0Info::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_183_0_11, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&Pi0Info::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_183_0_12, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&Pi0Info::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_183_0_16, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_183_0_17, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&Pi0Info::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_183_0_18, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&Pi0Info::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_183_0_19, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&Pi0Info::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_183_0_20, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&Pi0Info::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("Pi0Info", 629, G__lreco_dict_183_0_21, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_Pi0Info), -1, 0, 1, 1, 1, 0, "u 'Pi0Info' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~Pi0Info", 755, G__lreco_dict_183_0_22, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_183_0_23, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_Pi0Info), -1, 1, 1, 1, 1, 0, "u 'Pi0Info' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncTrackInfo(void) {
   /* TrackInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo));
   G__memfunc_setup("TrackInfo",897,G__lreco_dict_184_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_184_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("P",80,G__lreco_dict_184_0_3, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("pt",228,G__lreco_dict_184_0_4, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("costh",545,G__lreco_dict_184_0_5, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_184_0_6, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&TrackInfo::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_184_0_7, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&TrackInfo::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_184_0_8, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&TrackInfo::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_184_0_9, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&TrackInfo::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_184_0_13, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_184_0_14, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&TrackInfo::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_184_0_15, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&TrackInfo::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_184_0_16, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&TrackInfo::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_184_0_17, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&TrackInfo::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("TrackInfo", 897, G__lreco_dict_184_0_18, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo), -1, 0, 1, 1, 1, 0, "u 'TrackInfo' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~TrackInfo", 1023, G__lreco_dict_184_0_19, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_184_0_20, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo), -1, 1, 1, 1, 1, 0, "u 'TrackInfo' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncVertexInfo(void) {
   /* VertexInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_VertexInfo));
   G__memfunc_setup("VertexInfo",1034,G__lreco_dict_185_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_VertexInfo), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_185_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_185_0_3, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&VertexInfo::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_185_0_4, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&VertexInfo::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_185_0_5, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&VertexInfo::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_185_0_6, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&VertexInfo::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_185_0_10, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_185_0_11, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&VertexInfo::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_185_0_12, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&VertexInfo::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_185_0_13, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&VertexInfo::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_185_0_14, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&VertexInfo::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("VertexInfo", 1034, G__lreco_dict_185_0_15, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_VertexInfo), -1, 0, 1, 1, 1, 0, "u 'VertexInfo' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~VertexInfo", 1160, G__lreco_dict_185_0_16, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_185_0_17, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_VertexInfo), -1, 1, 1, 1, 1, 0, "u 'VertexInfo' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncTrackInfo2(void) {
   /* TrackInfo2 */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo2));
   G__memfunc_setup("TrackInfo2",947,G__lreco_dict_186_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo2), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_186_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("px",232,G__lreco_dict_186_0_3, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("py",233,G__lreco_dict_186_0_4, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("pz",234,G__lreco_dict_186_0_5, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("pt",228,G__lreco_dict_186_0_6, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_186_0_7, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&TrackInfo2::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_186_0_8, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&TrackInfo2::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_186_0_9, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&TrackInfo2::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_186_0_10, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&TrackInfo2::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_186_0_14, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_186_0_15, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&TrackInfo2::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_186_0_16, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&TrackInfo2::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_186_0_17, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&TrackInfo2::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_186_0_18, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&TrackInfo2::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("TrackInfo2", 947, G__lreco_dict_186_0_19, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo2), -1, 0, 1, 1, 1, 0, "u 'TrackInfo2' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~TrackInfo2", 1073, G__lreco_dict_186_0_20, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_186_0_21, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_TrackInfo2), -1, 1, 1, 1, 1, 0, "u 'TrackInfo2' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncH0Info(void) {
   /* H0Info */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__lreco_dictLN_H0Info));
   G__memfunc_setup("H0Info",516,G__lreco_dict_187_0_1, 105, G__get_linked_tagnum(&G__lreco_dictLN_H0Info), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Clear",487,G__lreco_dict_187_0_2, 121, -1, -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("px",232,G__lreco_dict_187_0_3, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("py",233,G__lreco_dict_187_0_4, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("pz",234,G__lreco_dict_187_0_5, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("pt",228,G__lreco_dict_187_0_6, 100, -1, -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__lreco_dict_187_0_7, 85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&H0Info::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__lreco_dict_187_0_8, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&H0Info::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__lreco_dict_187_0_9, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&H0Info::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__lreco_dict_187_0_10, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&H0Info::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__lreco_dictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__lreco_dict_187_0_14, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__lreco_dict_187_0_15, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&H0Info::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__lreco_dict_187_0_16, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&H0Info::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__lreco_dict_187_0_17, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&H0Info::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__lreco_dict_187_0_18, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&H0Info::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("H0Info", 516, G__lreco_dict_187_0_19, (int) ('i'), G__get_linked_tagnum(&G__lreco_dictLN_H0Info), -1, 0, 1, 1, 1, 0, "u 'H0Info' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~H0Info", 642, G__lreco_dict_187_0_20, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 0);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__lreco_dict_187_0_21, (int) ('u'), G__get_linked_tagnum(&G__lreco_dictLN_H0Info), -1, 1, 1, 1, 1, 0, "u 'H0Info' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}


/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfunclreco_dict() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

}

static void G__cpp_setup_global1() {

   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globallreco_dict() {
  G__cpp_setup_global0();
  G__cpp_setup_global1();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

}

static void G__cpp_setup_func1() {
}

static void G__cpp_setup_func2() {
}

static void G__cpp_setup_func3() {
}

static void G__cpp_setup_func4() {
}

static void G__cpp_setup_func5() {
}

static void G__cpp_setup_func6() {
}

static void G__cpp_setup_func7() {
}

static void G__cpp_setup_func8() {
}

static void G__cpp_setup_func9() {
}

static void G__cpp_setup_func10() {
}

static void G__cpp_setup_func11() {
}

static void G__cpp_setup_func12() {
}

static void G__cpp_setup_func13() {
}

static void G__cpp_setup_func14() {
}

static void G__cpp_setup_func15() {
}

static void G__cpp_setup_func16() {
}

static void G__cpp_setup_func17() {
}

static void G__cpp_setup_func18() {
}

static void G__cpp_setup_func19() {
}

static void G__cpp_setup_func20() {
}

static void G__cpp_setup_func21() {
}

static void G__cpp_setup_func22() {

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funclreco_dict() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
  G__cpp_setup_func2();
  G__cpp_setup_func3();
  G__cpp_setup_func4();
  G__cpp_setup_func5();
  G__cpp_setup_func6();
  G__cpp_setup_func7();
  G__cpp_setup_func8();
  G__cpp_setup_func9();
  G__cpp_setup_func10();
  G__cpp_setup_func11();
  G__cpp_setup_func12();
  G__cpp_setup_func13();
  G__cpp_setup_func14();
  G__cpp_setup_func15();
  G__cpp_setup_func16();
  G__cpp_setup_func17();
  G__cpp_setup_func18();
  G__cpp_setup_func19();
  G__cpp_setup_func20();
  G__cpp_setup_func21();
  G__cpp_setup_func22();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__lreco_dictLN_TClass = { "TClass" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_TBuffer = { "TBuffer" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_TMemberInspector = { "TMemberInspector" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_TObject = { "TObject" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR = { "vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR = { "reverse_iterator<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR = { "vector<TVirtualArray*,allocator<TVirtualArray*> >" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR = { "reverse_iterator<vector<TVirtualArray*,allocator<TVirtualArray*> >::iterator>" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_DalitzVars = { "DalitzVars" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_DeltaEMbc = { "DeltaEMbc" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_EvtInfo = { "EvtInfo" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_SFWMInfo = { "SFWMInfo" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_EvtShape = { "EvtShape" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_GammaInfo = { "GammaInfo" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_GenHepEvt = { "GenHepEvt" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_vectorlEintcOallocatorlEintgRsPgR = { "vector<int,allocator<int> >" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR = { "reverse_iterator<vector<int,allocator<int> >::iterator>" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_GenParticleInfo = { "GenParticleInfo" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_HamletTag = { "HamletTag" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_IPBoost = { "IPBoost" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_Ks0Info = { "Ks0Info" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_Pi0Info = { "Pi0Info" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_TrackInfo = { "TrackInfo" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_VertexInfo = { "VertexInfo" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_TrackInfo2 = { "TrackInfo2" , 99 , -1 };
G__linked_taginfo G__lreco_dictLN_H0Info = { "H0Info" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtablelreco_dict() {
  G__lreco_dictLN_TClass.tagnum = -1 ;
  G__lreco_dictLN_TBuffer.tagnum = -1 ;
  G__lreco_dictLN_TMemberInspector.tagnum = -1 ;
  G__lreco_dictLN_TObject.tagnum = -1 ;
  G__lreco_dictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR.tagnum = -1 ;
  G__lreco_dictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__lreco_dictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR.tagnum = -1 ;
  G__lreco_dictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__lreco_dictLN_DalitzVars.tagnum = -1 ;
  G__lreco_dictLN_DeltaEMbc.tagnum = -1 ;
  G__lreco_dictLN_EvtInfo.tagnum = -1 ;
  G__lreco_dictLN_SFWMInfo.tagnum = -1 ;
  G__lreco_dictLN_EvtShape.tagnum = -1 ;
  G__lreco_dictLN_GammaInfo.tagnum = -1 ;
  G__lreco_dictLN_GenHepEvt.tagnum = -1 ;
  G__lreco_dictLN_vectorlEintcOallocatorlEintgRsPgR.tagnum = -1 ;
  G__lreco_dictLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__lreco_dictLN_GenParticleInfo.tagnum = -1 ;
  G__lreco_dictLN_HamletTag.tagnum = -1 ;
  G__lreco_dictLN_IPBoost.tagnum = -1 ;
  G__lreco_dictLN_Ks0Info.tagnum = -1 ;
  G__lreco_dictLN_Pi0Info.tagnum = -1 ;
  G__lreco_dictLN_TrackInfo.tagnum = -1 ;
  G__lreco_dictLN_VertexInfo.tagnum = -1 ;
  G__lreco_dictLN_TrackInfo2.tagnum = -1 ;
  G__lreco_dictLN_H0Info.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtablelreco_dict() {

   /* Setting up class,struct,union tag entry */
   G__get_linked_tagnum_fwd(&G__lreco_dictLN_TClass);
   G__get_linked_tagnum_fwd(&G__lreco_dictLN_TBuffer);
   G__get_linked_tagnum_fwd(&G__lreco_dictLN_TMemberInspector);
   G__get_linked_tagnum_fwd(&G__lreco_dictLN_TObject);
   G__get_linked_tagnum_fwd(&G__lreco_dictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR);
   G__get_linked_tagnum_fwd(&G__lreco_dictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__lreco_dictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR);
   G__get_linked_tagnum_fwd(&G__lreco_dictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_DalitzVars),sizeof(DalitzVars),-1,28928,(char*)NULL,G__setup_memvarDalitzVars,G__setup_memfuncDalitzVars);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_DeltaEMbc),sizeof(DeltaEMbc),-1,28928,(char*)NULL,G__setup_memvarDeltaEMbc,G__setup_memfuncDeltaEMbc);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_EvtInfo),sizeof(EvtInfo),-1,28928,(char*)NULL,G__setup_memvarEvtInfo,G__setup_memfuncEvtInfo);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_SFWMInfo),sizeof(SFWMInfo),-1,28928,(char*)NULL,G__setup_memvarSFWMInfo,G__setup_memfuncSFWMInfo);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_EvtShape),sizeof(EvtShape),-1,28928,(char*)NULL,G__setup_memvarEvtShape,G__setup_memfuncEvtShape);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_GammaInfo),sizeof(GammaInfo),-1,28928,(char*)NULL,G__setup_memvarGammaInfo,G__setup_memfuncGammaInfo);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_GenHepEvt),sizeof(GenHepEvt),-1,28928,(char*)NULL,G__setup_memvarGenHepEvt,G__setup_memfuncGenHepEvt);
   G__get_linked_tagnum_fwd(&G__lreco_dictLN_vectorlEintcOallocatorlEintgRsPgR);
   G__get_linked_tagnum_fwd(&G__lreco_dictLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_GenParticleInfo),sizeof(GenParticleInfo),-1,28928,(char*)NULL,G__setup_memvarGenParticleInfo,G__setup_memfuncGenParticleInfo);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_HamletTag),sizeof(HamletTag),-1,28928,(char*)NULL,G__setup_memvarHamletTag,G__setup_memfuncHamletTag);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_IPBoost),sizeof(IPBoost),-1,28928,(char*)NULL,G__setup_memvarIPBoost,G__setup_memfuncIPBoost);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_Ks0Info),sizeof(Ks0Info),-1,28928,(char*)NULL,G__setup_memvarKs0Info,G__setup_memfuncKs0Info);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_Pi0Info),sizeof(Pi0Info),-1,28928,(char*)NULL,G__setup_memvarPi0Info,G__setup_memfuncPi0Info);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_TrackInfo),sizeof(TrackInfo),-1,28928,(char*)NULL,G__setup_memvarTrackInfo,G__setup_memfuncTrackInfo);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_VertexInfo),sizeof(VertexInfo),-1,28928,(char*)NULL,G__setup_memvarVertexInfo,G__setup_memfuncVertexInfo);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_TrackInfo2),sizeof(TrackInfo2),-1,28928,(char*)NULL,G__setup_memvarTrackInfo2,G__setup_memfuncTrackInfo2);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__lreco_dictLN_H0Info),sizeof(H0Info),-1,28928,(char*)NULL,G__setup_memvarH0Info,G__setup_memfuncH0Info);
}
extern "C" void G__cpp_setuplreco_dict(void) {
  G__check_setup_version(30051515,"G__cpp_setuplreco_dict()");
  G__set_cpp_environmentlreco_dict();
  G__cpp_setup_tagtablelreco_dict();

  G__cpp_setup_inheritancelreco_dict();

  G__cpp_setup_typetablelreco_dict();

  G__cpp_setup_memvarlreco_dict();

  G__cpp_setup_memfunclreco_dict();
  G__cpp_setup_globallreco_dict();
  G__cpp_setup_funclreco_dict();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfunclreco_dict();
  return;
}
class G__cpp_setup_initlreco_dict {
  public:
    G__cpp_setup_initlreco_dict() { G__add_setup_func("lreco_dict",(G__incsetup)(&G__cpp_setuplreco_dict)); G__call_setup_funcs(); }
   ~G__cpp_setup_initlreco_dict() { G__remove_setup_func("lreco_dict"); }
};
G__cpp_setup_initlreco_dict G__cpp_setup_initializerlreco_dict;

                                                                                                                                                                                                       mcdriver.cc_future                                                                                  0000644 0047266 0051066 00000021233 12770400276 014277  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                int b2d0pipi::BDecayTime_MC(const Gen_hepevt& b0,double& sigDT,double& tagDT,double& sigZ,double& tagZ,double& UpsZ,double& zups){
  if(!b0){
    sigDT = -999; sigZ = -998;
    tagDT = -999; tagZ = -998;
    return -1;
  }
  Gen_hepevt btag;
  if(TagB_MC(b0,btag)){
    sigDT = -999; sigZ = -999;
    tagDT = -999; tagZ = -999;
    return -1;
  }
  Gen_hepevt btagch = mc_list[btag.da(0)-1];
  if(!btagch){
    sigDT = -998; sigZ = -998;
    tagDT = -998; tagZ = -998;
    return -2;
  }
  Gen_hepevt bsigch = mc_list[b0.da(0)-1];
  if(!bsigch){
    sigDT = -997; sigZ = -997;
    tagDT = -997; tagZ = -997;
    return -3;
  }
  if(dump){
    cout << "Btag ch id = " << btagch.idhep();
    cout << ", Btag id = " << btag.idhep();
    cout << ", Bsig ch id = " << bsigch.idhep();
    cout << ", Bsig id = " << b0.idhep() << endl;
  }
  HepLorentzVector btagchLV = mc_4vert_b0(btagch);
  HepLorentzVector btagLV   = mc_4vert_b0(btag);
  HepLorentzVector tagLV = btagchLV - btagLV;
  tagZ = tagLV.z();
  HepLorentzVector bsigLV = mc_4vert_b0(b0);
  UpsZ = bsigLV.z();
  zups = UpsZ;
  if(abs(btagLV.z()-bsigLV.z())>1.e-4) cout << "tagZ = " << btagLV.z() << ", sigZ = " << bsigLV.z() << endl;
  HepLorentzVector sigLV = mc_4vert_d0(b0) - bsigLV;
  sigZ = sigLV.z();
  HepLorentzVector tagLM = mc_4mom_b0(btag);
  HepLorentzVector sigLM = mc_4mom_b0(b0);
  HepLorentzVector sumLM = tagLM+sigLM;
  Hep3Vector bv = -sumLM.boostVector();
  Hep3Vector BEbv = -BeamEnergy::CMBoost();
if(dump){
    cout << "My boost: (" << bv.x() << "," << bv.y() << "," << bv.z() << ")" << endl;
    cout << "BE boost: (" << BEbv.x() << "," << BEbv.y() << "," << BEbv.z() << ")" << endl;
  }
  tagLV.boost(bv);
  sigLV.boost(bv);
  sigDT = sigLV.t();
  tagDT = tagLV.t();
  return 0;
}

int b2d0pipi::TagB_MC(const Gen_hepevt& b0, Gen_hepevt& btag){
  const int b0id = b0.idhep();
  for(int i=0; i<mc_list.size(); i++){
    if(mc_list[i].idhep() == -b0id){
      btag = mc_list[i];
      return 0;
    }
  }
  return -1;
}

Gen_hepevt b2d0pipi::mc_get_d0(const Gen_hepevt& b0){
  if(abs(mc_list[b0.da(0)-1].idhep()) == 423){
    Gen_hepevt& dst0 = mc_list[b0.da(0)-1];
    if(abs(mc_list[dst0.da(0)-1].idhep()) == 421) return mc_list[dst0.da(0)-1];
    else return mc_list[dst0.da(0)];
  } else if(abs(mc_list[b0.da(0)].idhep()) == 423){
    Gen_hepevt& dst0 = mc_list[b0.da(0)];
    if(abs(mc_list[dst0.da(0)-1].idhep()) == 421) return mc_list[dst0.da(0)-1];
    else return mc_list[dst0.da(0)];
  } else{
    if(abs(mc_list[b0.da(0)-1].idhep()) == 421) return mc_list[b0.da(0)-1];
    else                                        return mc_list[b0.da(0)];
  }
}

Gen_hepevt b2d0pipi::mc_get_pip(const Gen_hepevt& b0){
  if(mc_list.at(mc_get_d0(b0).da(0)).idhep() == 211) return mc_list.at(mc_get_d0(b0).da(0));
  if(mc_list.at(mc_get_d0(b0).da(0)+1).idhep() == 211) return mc_list.at(mc_get_d0(b0).da(0)+1);
  return mc_list.at(mc_get_d0(b0).da(0)-1);
}
Gen_hepevt b2d0pipi::mc_get_pim(const Gen_hepevt& b0){
  if(mc_list.at(mc_get_d0(b0).da(0)).idhep() == -211) return mc_list.at(mc_get_d0(b0).da(0));
  if(mc_list.at(mc_get_d0(b0).da(0)+1).idhep() == -211) return mc_list.at(mc_get_d0(b0).da(0)+1);
  return mc_list.at(mc_get_d0(b0).da(0)-1);
}
Gen_hepevt b2d0pipi::mc_get_k0s(const Gen_hepevt& b0){
  if(abs(mc_list.at(mc_get_d0(b0).da(0)).idhep()) == 310) return mc_list.at(mc_get_d0(b0).da(0));
  if(abs(mc_list.at(mc_get_d0(b0).da(0)+1).idhep()) == 310) return mc_list.at(mc_get_d0(b0).da(0)+1);
  return mc_list.at(mc_get_d0(b0).da(0)-1);
}
double b2d0pipi::DDecayTime_MC(const Gen_hepevt& b0){
  HepLorentzVector d4r = mc_4vert_k0s(b0) - mc_4vert_d0(b0);
  Hep3Vector bv = -mc_4mom_d0(b0).boostVector();
  d4r.boost(bv);
  return d4r.t();
}

HepLorentzVector b2d0pipi::mc_4vert_b0(const Gen_hepevt& b0){
  if(dump){
    cout << b0 << " (";
    cout << b0.idhep() << ") : (";
    cout << b0.V(0) << ",";
    cout << b0.V(1) << ",";
    cout << b0.V(2) << ",";
    cout << b0.V(3) << ")" << endl;
  }
  return HepLorentzVector(b0.V(0),b0.V(1),b0.V(2),b0.V(3));
}
HepLorentzVector b2d0pipi::mc_4vert_d0(const Gen_hepevt& b0){
  const Gen_hepevt& d0 = mc_get_d0(b0);
  return HepLorentzVector(d0.V(0),d0.V(1),d0.V(2),d0.V(3));
}
HepLorentzVector b2d0pipi::mc_4vert_k0s(const Gen_hepevt& b0){
  const Gen_hepevt& k0s = mc_get_k0s(b0);
  return HepLorentzVector(k0s.V(0),k0s.V(1),k0s.V(2),k0s.V(3));
}
HepLorentzVector b2d0pipi::mc_4vert_pip(const Gen_hepevt& b0){
  const Gen_hepevt& pip = mc_get_pip(b0);
  return HepLorentzVector(pip.V(0),pip.V(1),pip.V(2),pip.V(3));
}
HepLorentzVector b2d0pipi::mc_4mom_b0(const Gen_hepevt& b0){
  return HepLorentzVector(b0.P(0),b0.P(1),b0.P(2),b0.P(3));
}
HepLorentzVector b2d0pipi::mc_4mom_d0(const Gen_hepevt& b0){
  const Gen_hepevt& d0 = mc_get_d0(b0);
  return HepLorentzVector(d0.P(0),d0.P(1),d0.P(2),d0.P(3));
}
HepLorentzVector b2d0pipi::mc_4mom_pip(const Gen_hepevt& b0){
  const Gen_hepevt& pip = mc_get_pip(b0);
  return HepLorentzVector(pip.P(0),pip.P(1),pip.P(2),pip.P(3));
}
HepLorentzVector b2d0pipi::mc_4mom_pim(const Gen_hepevt& b0){
  const Gen_hepevt& pim = mc_get_pim(b0);
  return HepLorentzVector(pim.P(0),pim.P(1),pim.P(2),pim.P(3));
}
HepLorentzVector b2d0pipi::mc_4mom_k0s(const Gen_hepevt& b0){
  const Gen_hepevt& k0s = mc_get_k0s(b0);
  return HepLorentzVector(k0s.P(0),k0s.P(1),k0s.P(2),k0s.P(3));
}

double b2d0pipi::z_asc_mc(const Particle& _b0){
  if(!_b0.relation().genHepevt()) return -999;
  const int b0id = _b0.relation().genHepevt().idhep();
  for(int i=0; i<mc_list.size(); i++){
    if(mc_list[i].idhep() == -b0id){
      return mc_list[mc_list[i].da(0)-1].V(3);
    }
  }
  return 999;
}

//double b2d0pipi::t_asc_mc(const Particle& _b0){
//  if(!_b0.relation().genHepevt()) return -999;
//  const int b0id = _b0.relation().genHepevt().idhep();
//  for(int i=0; i<mc_list.size(); i++){
//    if(mc_list[i].idhep() == -b0id){
//      return mc_list[mc_list[i].da(0)-1].V(0);
//    }
//  }
//  return 999;
//}

int b2d0pipi::GetMpMm_MC(const Gen_hepevt& b0,double& mp, double& mm){
  const HepLorentzVector& mom_pip = mc_4mom_pip(b0);
  const HepLorentzVector& mom_pim = mc_4mom_pim(b0);
  const HepLorentzVector& mom_k0s = mc_4mom_k0s(b0);

  HepLorentzVector s = mom_pip+mom_k0s;
  mp = s.m2();
  s = mom_pim+mom_k0s;
  mm = s.m2();

  return 0;
}

int b2d0pipi::NPTag(Particle& _b0){
  if(!_b0.relation().genHepevt()) return 0;
  const Gen_hepevt& b0 = _b0.relation().genHepevt();
  Gen_hepevt b0tag;
  const int b0id = _b0.relation().genHepevt().idhep();
//  cout << "b0id = " << b0id << ", size = " << mc_list.size() << endl;
  int i=0;
  for(i=0; i<mc_list.size(); i++){
    if(mc_list[i].idhep() == -b0id){
      b0tag = mc_list[i];
      break;
    }
  }
  if(i == mc_list.size()){
    cout << "Can't find tag B0" << endl;
    return -1;
  }
  for(i = b0tag.daFirst(); i <= b0tag.daLast(); i++){
    if(!i) continue;
    const Gen_hepevt& da = mc_list[i-1];
    int did = abs(da.idhep());
//    cout << "da" << i << ": " << did << endl;
    if(!did) continue;
    if(did == 411 || did == 421) return 1;
    for(int j = da.daFirst(); j <= da.daLast(); j++){
      if(!j) continue;
      const Gen_hepevt& dda = mc_list[j-1];
      int ddid = abs(dda.idhep());
//      cout << " dda" << j << ": " << ddid << endl;
      if(ddid == 411 || ddid == 421) return 1;
      for(int k = dda.daFirst(); k <= dda.daLast(); k++){
        if(!k) continue;
        const Gen_hepevt& ddda = mc_list[k-1];
        int dddid = abs(ddda.idhep());
//        cout << "  ddda" << k << ": " << dddid << endl;
        if(dddid == 411 || dddid == 421) return 1;
      }
    }
  }
  return 0;
}

/////////////////////
//   MC Functions  //
/////////////////////
int b2d0pipi::IsSignalD(const Particle& _d0,int& mode, int& h0mode, int& flv, double& z, double& t, double& td0, double& mp, double& mm, double& zups){
  mode = 0; h0mode = 0; flv = 0;
  if(!_d0.relation().genHepevt()) return -1;
  const Gen_hepevt& d0 = _d0.relation().genHepevt();
  if(!d0.moFirst()) return -5;
  const Gen_hepevt& b0 = mc_list[d0.moFirst()-1];
  if(abs(b0.idhep()) != 511) return -4;
  if((b0.daLast()-b0.daFirst())>1) return -2;
  const Gen_hepevt& h0 = (mc_list[b0.daFirst()-1].idhep() ==  d0.idhep()) ? mc_list[b0.daLast()-1] : mc_list[b0.daFirst()-1];
  switch(h0.idhep()){
  case 111:// pi0
    mode = 1;
    h0mode = 10;
    break;
  case 113:// rho(770)
    mode = 4;
    h0mode = 40;
    break;
  case 221:// eta
    mode = 2;
    h0mode = (h0.daLast()-h0.daFirst()) == 1 ? 10 : 20;
    break;
  case 223:// omega
    mode = 3;
    h0mode = 20;
    break;
  default:
    return -3;
  }
  b0.idhep() == 511 ? flv = 1 : flv = -1;
  double tasc,zasc,zup;
  BDecayTime_MC(b0,t,tasc,z,zasc,zup,zups);
  td0 = DDecayTime_MC(b0);
  GetMpMm_MC(b0,mp,mm);
  if(b0.idhep() == -511) flv = -1;
  else                   flv = 1;
  return 0;
}

                                                                                                                                                                                                                                                                                                                                                                     particledriver.cc_future                                                                            0000644 0047266 0051066 00000001005 12770400276 015476  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "particledriver.h"

const Particle& b2d0pipi::get_d0(const Particle& b0){
  if(b0.child(0).lund() != 423) return b0.child(0);
  else return b0.child(0).child(0);
}
const Particle& b2d0pipi::get_pip(const Particle& b0){
  const Particle& d0 = get_d0(b0);
  return d0.child(1);
}
const Particle& b2d0pipi::get_pim(const Particle& b0){
  const Particle& d0 = get_d0(b0);
  return d0.child(2);
}
const Particle& b2d0pipi::get_k0s(const Particle& b0){
  const Particle& d0 = get_d0(b0);
  return d0.child(0);
}


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           phiinfo.cc                                                                                          0000644 0047266 0051066 00000001243 12770400276 012525  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "phiinfo.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///////////////
//    phi    //
///////////////
PhiUserInfo::PhiUserInfo() :
  m_mass(0),
  m_chi2(-1),
  m_mcflag(0)
{
}

PhiUserInfo::~PhiUserInfo()
{
}

PhiUserInfo::PhiUserInfo(const PhiUserInfo &x)
  : m_mass(x.m_mass),
    m_chi2(x.m_chi2),
    m_mcflag(x.m_mcflag)
{
}

PhiUserInfo* PhiUserInfo::clone(void) const
{
  PhiUserInfo *x = new PhiUserInfo( *this );
  return x;
}

PhiUserInfo & PhiUserInfo::operator = (const PhiUserInfo &x)
{
  m_mass   = x.m_mass;
  m_chi2   = x.m_chi2;
  m_mcflag = x.m_mcflag;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
                                                                                                                                                                                                                                                                                                                                                             pi0info.cc                                                                                          0000644 0047266 0051066 00000001616 12770400276 012441  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "pi0info.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///////////////
//    pi0    //
///////////////
Pi0UserInfo::Pi0UserInfo() :
  m_mass(0),
  m_chi2(0),
  m_fit_flag(-1),
  m_gamma_energy(0),
  m_mode(-1),
  m_gamma_pt(0)
{
}

Pi0UserInfo::~Pi0UserInfo()
{
}

Pi0UserInfo::Pi0UserInfo(const Pi0UserInfo &x)
  : m_mass(x.m_mass),
    m_chi2(x.m_chi2),
    m_gamma_energy(x.m_gamma_energy),
    m_gamma_pt(x.m_gamma_pt),
    m_fit_flag(x.m_fit_flag),
    m_mode(x.m_mode)
{
}

Pi0UserInfo* Pi0UserInfo::clone(void) const
{
  Pi0UserInfo *x = new Pi0UserInfo( *this );
  return x;
}

Pi0UserInfo & Pi0UserInfo::operator = (const Pi0UserInfo &x)
{
  m_mass = x.m_mass;
  m_chi2 = x.m_chi2;
  m_gamma_energy = x.m_gamma_energy;
  m_gamma_pt = x.m_gamma_pt;
  m_fit_flag = x.m_fit_flag;
  m_mode = x.m_mode;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
                                                                                                                  Pi0Info.cc                                                                                          0000644 0047266 0051066 00000001137 12770400276 012337  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "Pi0Info.h"
#include <cmath>

ClassImp(Pi0Info)

void Pi0Info::Clear(void){
  m = 0, m_raw = 0;
  p[0] = -99, p[1] = -99, p[2] = -99;
  eg1 = -99, eg2 = -99;
  chisq = -1;
}

double Pi0Info::P(void) const{
  return sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
}

double Pi0Info::pt(void) const{
  return sqrt(p[0]*p[0]+p[1]*p[1]);
}

double Pi0Info::costh(void) const{
  return p[2]/P();
}

double Pi0Info::asym(void) const{
  return (egmax()-egmin())/(eg1+eg2);
}

double Pi0Info::egmin(void) const{
  return eg1 > eg2 ? eg2 : eg1;
}

double Pi0Info::egmax(void) const{
  return eg1 > eg2 ? eg1 : eg2;
}

                                                                                                                                                                                                                                                                                                                                                                                                                                 recotools.cc                                                                                        0000644 0047266 0051066 00000042765 13064466016 013121  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include <cmath>

#include "recotools.h"
#include "uisetter.h"

#include "benergy/BeamEnergy.h"
#include <CLHEP/Vector/LorentzVector.h>
#include "particle/utility.h"
#include "particle/combination.h"
#include "hamlet/Hamlet.h"
#include "hamlet/Fbtag_MultDimLikelihood0.h"
#include "hamlet/Fbtag_NN1.h"
#include "tagv/TagV.h"
#include "ip/IpProfile.h"
#include "geninfo.h"

#include "toolbox/FuncPtr.h"
#include "toolbox/FoxWolfr.h"
#include "toolbox/Thrust.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif

  // Energy threshold of gamma shower in GeV.
double RTools::eth_barrel = 0.05;
double RTools::eth_fwdec  = 0.10;
double RTools::eth_bwdec  = 0.10;
  // Gaps between barrel and endcaps in cos(theta).
double RTools::cos_fwd_gap =  0.8501;
double RTools::cos_bwd_gap = -0.6643;


using std::sqrt;
using std::fabs;
using std::abs;
using std::vector;

typedef HepLorentzVector lvect;
typedef Hep3Vector vect;

double RTools::Helicity(const Particle& p, const int nch) {
    return Helicity(p.p(), p.child(nch).p());
}

double RTools::Helicity(const lvect& mov, lvect chv) {
    chv.boost(-mov.boostVector());
    const vect ch3v = chv.vect().unit();
    const vect mo3v = mov.vect().unit();
    return ch3v.dot(mo3v);
}

double RTools::ResHelicity(const Particle& b0, int nch1, int nch2,
                           double& invmsq) {
    return ResHelicity(b0.p(), b0.child(nch1).p(), b0.child(nch2).p(), invmsq);
}

double RTools::ResHelicity(const lvect& mov, const lvect& ch1v, const lvect& ch2v,
                           double& invmsq) {
    lvect ch1vc = ch1v;
    lvect momvc = mov;
    const lvect sum_lv = ch1v+ch2v;
    const vect h0p3 = -sum_lv.boostVector();
    invmsq = sum_lv.m2();
    ch1vc.boost(h0p3);
    momvc.boost(h0p3);

    const vect ch1_p3 = ch1vc.vect().unit();
    const vect mom_p3 = momvc.vect().unit();
    return ch1_p3.dot(mom_p3);
}

void RTools::Mbc_deltaE(const Particle& b0, double& mbc, double& de) {
    lvect lv = b0.p();
    lv.boost(-BeamEnergy::CMBoost());
    const double benergy = BeamEnergy::E_beam_corr();
    de = lv.t() - benergy;
    lv.setT(benergy);
    mbc = lv.mag();
}

double RTools::cosThetaCMS(const Particle& b0) {
    lvect p = b0.p();
    p.boost(-BeamEnergy::CMBoost());
    return p.cosTheta();
}

double RTools::MissingMass(const Particle& b0) {
    lvect p(0, 0, 0, b0.pType().mass());
    p.boost(BeamEnergy::CMBoost());
    p -= b0.child(0).p();
    return p.m();
}

//double RTools::BDecayTime(const Particle& b0){
//  const double mb0 = b0.pType().mass();
//  vect dr = b0.x()-IpProfile::position(1);
//  return dr*b0.p3()*mb0/b0.p3().mag2();
//}

//bool RTools::CheckSVD(const Particle& pi){ 
//  Mdst_trk_fit &trk_fit = pi.mdstCharged().trk().mhyp(2);//2 -> pi 
//  if(trk_fit.nhits(3)<1 || trk_fit.nhits(4)<2) return false;// 3 -> rphi, 4 -> z 
//  return true; 
//}

bool RTools::IsTheSame(const Particle& p1, const Particle& p2) {
    if(fabs(p1.charge()) != fabs(p2.charge())) return false;
    const vect dp = p1.p().vect() - p2.p().vect();
    return dp.mag() < 0.0001 ? true : false;
}

//bool RTools::IsDuplicated(const Particle& b0){
//  const Particle& pi1    = b0.child(1);// pi+
//  const Particle& pi2    = b0.child(2);// pi-
//  const Particle& d0     = b0.child(0);
//  const Particle& d0_pip = d0.child(1);
//  const Particle& d0_pim = d0.child(2);
//  const Particle& ks     = d0.child(0);
//  const Particle& ks_pip = ks.child(0).lund() > 0 ? ks.child(0) : ks.child(1);
//  const Particle& ks_pim = ks.child(0).lund() > 0 ? ks.child(1) : ks.child(0);
//  if(IsTheSame(d0_pip,ks_pip)) return true;
//  if(IsTheSame(d0_pim,ks_pim)) return true;
//  if(IsTheSame(pi1,ks_pip))    return true;
//  if(IsTheSame(pi2,ks_pim))    return true;
//  if(IsTheSame(pi1,d0_pip))    return true;
//  if(IsTheSame(pi2,d0_pim))    return true;
//  return false;
//}

bool RTools::IsDuplicated(const Particle& p) {
    vector<Particle*> fspl = p.relation().finalStateParticles();
    for (unsigned i = 0; i < fspl.size(); i++) {
        for (unsigned j = i + 1; j < fspl.size(); j++) {
            if (checkSame(*fspl[i], *fspl[j])) return true;
        }
    }
    return false;
}

double RTools::CosThr(const Particle& b0, double& thr_sig, double& thr_oth) {
    vector<vect> sigtrk, otherb;
    RTools::BoostedList(b0, sigtrk, otherb);

    vect thrSig = thrust(sigtrk.begin(), sigtrk.end(), SelfFunc(vect()));
    vect thrOth = thrust(otherb.begin(), otherb.end(), SelfFunc(vect()));
    thr_sig = thrSig.mag();
    thr_oth = thrOth.mag();
    return thrSig.unit().dot(thrOth.unit());
}

void RTools::BoostedList(const Particle& b0,
                        vector<vect>&sigtrk, vector<vect>&otherb) {
    vector<Particle> pi, pi_m, kpl, kml, allgams;
    makeKPi(kpl, kml, pi, pi_m, false);
    makeGamma(allgams);

    vector<Particle*> final = b0.relation().finalStateParticles();
    pi.insert(pi.end(), pi_m.begin(), pi_m.end());
    pi.insert(pi.end(), allgams.begin(), allgams.end());

    for (int i = 0; i < b0.relation().nFinalStateParticles(); i++)
        removeParticle(pi,b0.relation().finalStateParticle(i));

    const vect boostv = -BeamEnergy::CMBoost();
    sigtrk.clear(); otherb.clear();

    for (unsigned i = 0; i < final.size(); i++) {
        lvect p = final[i]->p();
        p.boost(boostv);
        sigtrk.push_back(p.vect());
    }

    for (unsigned i = 0; i < pi.size(); i++) {
        lvect p = pi[i].p();
        p.boost(boostv);
        otherb.push_back(p.vect());
    }
}

int RTools::TagVertex(Particle& b0) {
    TagVK tagv;
    tagv.setdefault(b0, b0.x());

    vector<Particle> pipl, piml, kpl, kml;
    makeKPi(kpl, kml, pipl, piml);
    pipl.insert(pipl.end(), piml.begin(), piml.end());
    for (int i = 0; i < b0.relation().nFinalStateParticles(); i++)
        removeParticle(pipl, b0.relation().finalStateParticle(i));
    for (unsigned i = 0; i < pipl.size(); i++) tagv.push_back(&pipl[i]);
    if (tagv.fit()) return -1;
    if (tagv.chisq_tracks() < 0) return -2;
    UISetter::SetTagVtxFitInfo(b0, tagv);
    return 0;
}

void RTools::make_MDLH_b0_tag(Particle &b0) {
    Hamlet hamlet;
    hamlet.setBcp(b0);
    hamlet.setTagMethod(Hamlet::MULT_DIM_LH);
    const Fbtag_MultDimLikelihood0 &mdlh_evtgen = hamlet.fbtg_mult_dim_likelihood();
    dynamic_cast<B0UserInfo&>(b0.userInfo()).fqLH(mdlh_evtgen.fq());
    dynamic_cast<B0UserInfo&>(b0.userInfo()).fqErrLH(mdlh_evtgen.fq_err());
    dynamic_cast<B0UserInfo&>(b0.userInfo()).qLH(hamlet.q());
}

void RTools::make_NN_b0_tag(Particle &b0) { 
    Hamlet hamlet;
    hamlet.setBcp(b0);
    hamlet.setTagMethod(Hamlet::NN1);
    const Fbtag_NN1 &nn_evtgen = hamlet.fbtg_NN1();
//  dynamic_cast<B0UserInfo&>(b0.userInfo()).fqNN(nn_evtgen.fq());
//  dynamic_cast<B0UserInfo&>(b0.userInfo()).fqErrNN(nn_evtgen.fq_err());
//  dynamic_cast<B0UserInfo&>(b0.userInfo()).qNN(hamlet.q());
}

//===
// Select gamma exceeding proper threshold.
// return value, -1:bad, 0:good.
//===
int RTools::gamma_tight(vect gamma_3v) {
    if (gamma_3v.cosTheta() > cos_fwd_gap) {  // In Fwd. Endcap.
        if (gamma_3v.mag() > eth_fwdec) return 0;
    } else if (gamma_3v.cosTheta() < cos_fwd_gap &&
               gamma_3v.cosTheta() > cos_bwd_gap) {  // In Barrel.
        if (gamma_3v.mag() > eth_barrel) return 0;
    } else {  // In Bwd. Endcap.
        if (gamma_3v.mag() > eth_bwdec) return 0;
    }
    return -1;
}

void RTools::KspipiDalitzVars(const Particle& d0, double& mpsq, double& mmsq) {
    const lvect momp = d0.child(0).p() + d0.child(1).p();
    const lvect momm = d0.child(0).p() + d0.child(2).p();
    mpsq = momp.m2();
    mmsq = momm.m2();
}

bool RTools::MyCheckSame(const Particle &p1, const Particle &p2) {
  // same     --> return true;
  // not same --> return false;
    if (p1.nChildren() == 0 && p2.nChildren() == 0) {
    /* p1 and p2 have no children */
        return p1.relation().isIdenticalWith(p2.relation());
    } else if (p1.nChildren() > 0 && p2.nChildren() == 0) {
    /* p1 have children and no p2 children */
        for (unsigned i1 = 0; i1 < p1.nChildren(); ++i1)
            if (MyCheckSame(p1.child(i1), p2)) return true;
        return false;
    } else if (p1.nChildren() == 0 && p2.nChildren() > 0) {
    /* p2 have children and no p1 children */
        for (unsigned i2 = 0; i2 < p2.nChildren(); ++i2)
            if(MyCheckSame(p1, p2.child(i2))) return true;
        return false;
    } else {
    /* p1 and p2 both have children */
        for (unsigned i1 = 0; i1 < p1.nChildren(); ++i1)
           for (unsigned i2 = 0; i2 < p2.nChildren(); ++i2)
               if (MyCheckSame(p1.child(i1), p2.child(i2))) return true;
        return false;
    }
}

void RTools::MyCombination(vector<Particle> &new_p,
                           const Ptype &ptype,
                           vector<Particle> &p1, vector<Particle> &p2) {
    for (unsigned i = 0; i < p1.size(); i++)
        for (unsigned j = 0; j < p2.size(); j++)
            MyCombinationNoCut(new_p, ptype, p1[i], p2[j]);
}

void RTools::MyCombination(vector<Particle> &new_p,
                           const Ptype &ptype,
                           vector<Particle> &p1, vector<Particle> &p2,
                           const double &massL,const double &massR) {
    for (unsigned i = 0; i < p1.size(); i++)
        for (unsigned j = 0; j < p2.size(); j++)
            MyCombinationCut(new_p, ptype, p1[i], p2[j], massL, massR);
}

void RTools::MyCombinationNoCut(vector<Particle> &new_p,
                                const Ptype &ptype,
                                Particle &p1, Particle &p2) {
    if (MyCheckSame(p1, p2)) return;
    Particle cand(p1.p() + p2.p(), ptype);
    cand.relation().append(p1);
    cand.relation().append(p2);
    new_p.push_back(cand);
}

void RTools::MyCombinationCut(vector<Particle> &new_p,
                              const Ptype &ptype,
                              Particle &p1, Particle &p2,
                              const double &massL, const double &massR) {
    if (MyCheckSame(p1, p2)) return;
  //...checks mass
    double mass = (p1.p() + p2.p()).m();
    if ((massL > mass) || (mass > massR)) return;
    Particle cand(p1.p() + p2.p(),ptype);
    cand.relation().append(p1);
    cand.relation().append(p2);
    new_p.push_back(cand);
}

int RTools::ChldIndex(const Particle& p, const int ch, const bool absflag) {
    const int nch = p.nChildren();
    int i = 0;
    for (i = 0; i < nch; i++) {
        const int lund = absflag ? abs(p.child(i).lund()) : p.child(i).lund();
        if(ch == lund) break;
    }
    return i == nch ? -1 : i;
}

int RTools::GetTrueVertices(const Gen_hepevt& sigb,
                            const vector<Gen_hepevt>& mcl,
                            double& sigv, double& tagv, double& upsv) {
    if (!sigb) return -1;
    const Gen_hepevt& tagb = OtherB(sigb, mcl);
    if (tagb == sigb) return -2;

    const Gen_hepevt& tagbch = mcl[tagb.da(0)-1];
    if (!tagbch) return -3;

    const Gen_hepevt& sigbch = mcl[sigb.da(0)-1];
    if (!sigbch) return -4;

    upsv = sigb.V(3);
    sigv = sigbch.V(3) - upsv;
    tagv = tagbch.V(3) - upsv;

    return 0;
}

const Gen_hepevt& RTools::OtherB(const Gen_hepevt& sigb,
                                 const vector<Gen_hepevt>& mcl) {
    const int sigid = sigb.idhep();
    for (unsigned i = 0; i < mcl.size(); i++)
        if (mcl[i].idhep() == -sigid) return mcl[i];
    return sigb;
}

lvect RTools::mc_lvtx(const Particle& b) {
    const Gen_hepevt& bgh = const_cast<Gen_hepevt&>(b.relation().genHepevt());
    return !bgh ? lvect(0) : mc_lvtx(bgh);
}

lvect RTools::mc_lmom(const Particle& b) {
    const Gen_hepevt& bgh = const_cast<Gen_hepevt&>(b.relation().genHepevt());
    return !bgh ? lvect(0) : mc_lmom(bgh);
}

lvect RTools::mc_lvtx(const Gen_hepevt& b0) {
    return lvect(b0.V(0), b0.V(1), b0.V(2), b0.V(3)); 
}

lvect RTools::mc_lmom(const Gen_hepevt& b0) {
    return lvect(b0.P(0), b0.P(1), b0.P(2), b0.P(3)); 
}

const Gen_hepevt& RTools::mc_child(const Gen_hepevt& mo,
                                   const vector<Gen_hepevt>& mcl,
                                   const int ch, const bool absflag) {
    if (!mo) return mo;
    for (int i = mo.daFirst(); i <= mo.daLast(); i++) {
        if (absflag) {
            if (abs(mcl[i-1].idhep()) == ch) return mcl[i-1];
        } else {
            if (mcl[i-1].idhep() == ch) return mcl[i-1];
        }
    }
    return mo;
}

int RTools::mc_list(vector<Gen_hepevt>& mcl) {
    mcl.clear();
    Gen_hepevt_Manager &mGHep = Gen_hepevt_Manager::get_manager();
    for (vector<Gen_hepevt>::iterator i = mGHep.begin(); i != mGHep.end(); i++) {
        Gen_hepevt tmp = *i;
        mcl.push_back(tmp);
    }
    return mcl.size();
}

void RTools::FillKs0(const Particle& ks, Ks0Info& info) {
    info.m     = ks.p().m();
    info.p[0]  = ks.p().x();
    info.p[1]  = ks.p().y();
    info.p[2]  = ks.p().z();
    const KsUserInfo& ksinfo = static_cast<const KsUserInfo&>(ks.userInfo());
    info.dr    = ksinfo.dr();
    info.dz    = ksinfo.dz();
    info.dphi  = ksinfo.dphi();
    info.fl    = ksinfo.fl();
    info.m_raw = ksinfo.Mass();
}

void RTools::FillPi0(const Particle& pi0, Pi0Info& info) {
    info.m     = pi0.p().m();
    info.p[0]  = pi0.p().x();
    info.p[1]  = pi0.p().y();
    info.p[2]  = pi0.p().z();
    info.eg1   = pi0.child(0).p().t();
    info.eg2   = pi0.child(1).p().t();
    const Pi0UserInfo& pi0info = (Pi0UserInfo&)pi0.userInfo();
    info.m_raw = pi0info.Mass();
    info.chisq = pi0info.Chi2();
}

void RTools::FillKSFW(ksfwmoments& km, SFWMInfo& info, const int usefs) {
    km.usefinal(usefs);
    info.kmm2   = km.mm2();
    info.ket    = km.et();
    info.khso00 = km.Hso(0,0);
    info.khso01 = km.Hso(0,1);
    info.khso02 = km.Hso(0,2);
    info.khso03 = km.Hso(0,3);
    info.khso04 = km.Hso(0,4);
    info.khso10 = km.Hso(1,0);
    info.khso12 = km.Hso(1,2);
    info.khso14 = km.Hso(1,4);
    info.khso20 = km.Hso(2,0);
    info.khso22 = km.Hso(2,2);
    info.khso24 = km.Hso(2,4);
    info.khoo0  = km.Hoo(0);
    info.khoo1  = km.Hoo(1);
    info.khoo2  = km.Hoo(2);
    info.khoo3  = km.Hoo(3);
    info.khoo4  = km.Hoo(4);
}

void RTools::FillBVtx(const B0UserInfo& binfo,
                      VertexInfo& recvtx, VertexInfo& ascvtx) {
    const HepPoint3D& IP = IpProfile::e_position();
    recvtx.z     = binfo.ZSig()-IP.z();
    ascvtx.z     = binfo.ZAsc()-IP.z();
    recvtx.sz    = binfo.SzSig();
    ascvtx.sz    = binfo.SzAsc();
    recvtx.ntrk  = binfo.NtrkSig();
    ascvtx.ntrk  = binfo.NtrkAsc();
    recvtx.ndf   = binfo.NdfSig();
    ascvtx.ndf   = binfo.NdfAsc();
    recvtx.chisq = binfo.ChisqSig();
    ascvtx.chisq = binfo.ChisqAsc();
}

void RTools::FillTrk(const Particle& trk,TrackInfo& info) {
    info.p[0] = trk.p().x();
    info.p[1] = trk.p().y();
    info.p[2] = trk.p().z();
    const TrkUserInfo& trkinfo = static_cast<const TrkUserInfo&>(trk.userInfo());
    info.z        = trkinfo.z();
    info.r        = trkinfo.r();
    info.rz_svd   = trkinfo.rz_svd_hits();
    info.rphi_svd = trkinfo.rphi_svd_hits();
    info.atckpi   = trkinfo.atckpi();
    info.atckp    = trkinfo.atckp();
    info.eid      = trkinfo.eid();
    info.muid     = trkinfo.muid();
}

void RTools::FillTrk(const Particle& trk, TrackInfo2& info) {
    const TrkUserInfo& trkinfo = static_cast<const TrkUserInfo&>(trk.userInfo());
    info.p = trk.p().rho();
    info.costh = trk.p().cosTheta();
    info.phi = trk.p().phi();
    info.z        = trkinfo.z();
    info.r        = trkinfo.r();
    info.rz_svd   = trkinfo.rz_svd_hits();
    info.rphi_svd = trkinfo.rphi_svd_hits();
    info.atckpi   = trkinfo.atckpi();
    info.atckp    = trkinfo.atckp();
    info.eid      = trkinfo.eid();
    info.muid     = trkinfo.muid();

}

void RTools::FillH0(const Particle& h, H0Info& info) {
    info.p = h.p().rho();
    info.costh = h.p().cosTheta();
    info.phi = h.p().phi();
}

void RTools::FillGamma(const Particle& g, GammaInfo& info) {
    info.p[0] = g.p().x();
    info.p[1] = g.p().y();
    info.p[2] = g.p().z();
}

void RTools::FillShape(const Particle& b, EvtShape& shape) {
    ksfwmoments km(b, BeamEnergy::E_beam_corr(), -BeamEnergy::CMBoost());
    FillKSFW(km, shape.k0, 0);
    FillKSFW(km, shape.k1, 1);
    shape.cos_thr = CosThr(b, shape.thr_sig, shape.thr_oth);
}

void RTools::FillEvtInfo(EvtInfo& info) {
    Belle_event_Manager& evtmgr = Belle_event_Manager::get_manager();
    Belle_event& evthead = *evtmgr.begin();
    info.exp = evthead.ExpNo();
    info.run = evthead.RunNo();
    info.evtn = (evthead.EvtNo() & 0xfffffff);
}

void RTools::FillIPBoost(IPBoost& ipb) {
    const HepPoint3D IP = IpProfile::usable() ? IpProfile::e_position() : HepPoint3D(0);
    ipb.ip[0] = IP.x(); ipb.ip[1] = IP.y(); ipb.ip[2] = IP.z();
    ipb.boost[0] = -BeamEnergy::CMBoost().x();
    ipb.boost[1] = -BeamEnergy::CMBoost().y();
    ipb.boost[2] = -BeamEnergy::CMBoost().z();
}

void RTools::FillHamletTag(Particle& b0, HamletTag& ht) {
    make_MDLH_b0_tag(b0);
    const B0UserInfo& binfo = static_cast<const B0UserInfo&>(b0.userInfo());
    ht.tag_LH     = binfo.fqLH();
    ht.tag_LH_err = binfo.fqErrLH();
    ht.tagl       = binfo.tagl();
    ht.taglid     = binfo.taglid();
}

void RTools::FillGenHepEvt(GenHepEvt& ghe, const int maxsize) {
    ghe.idhep.clear(); ghe.daF.clear(); ghe.daL.clear();
    vector<Gen_hepevt> mcl;
    mc_list(mcl);
    for (unsigned i = 0; i < mcl.size() && i < maxsize; i++) {
        ghe.idhep.push_back(mcl[i].idhep());
        ghe.daF.push_back(mcl[i].daFirst());
        ghe.daL.push_back(mcl[i].daLast());
    }
}

bool RTools::FillGenPInfo(const Particle& p, GenParticleInfo& info) {
    Particle p1 = p;
    info.id = IDhep(p1);
    info.flag = getMCtruthFlag(p1);
    genDecayChain(p1, info.chain);
    return (info.flag == 1 || info.flag == 5 || info.flag == 10);
}

#if defined(BELLE_NAMESPACE)
}
#endif

           SFWMInfo.cc                                                                                         0000644 0047266 0051066 00000000552 12770400276 012463  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "SFWMInfo.h"

ClassImp(SFWMInfo)

void SFWMInfo::Clear(void){
  kmm2   = -99;
  ket    = -99;
  khso00 = -99;
  khso01 = -99;
  khso02 = -99;
  khso03 = -99;
  khso04 = -99;
  khso10 = -99;
  khso12 = -99;
  khso14 = -99;
  khso20 = -99;
  khso22 = -99;
  khso24 = -99;
  khoo0  = -99;
  khoo1  = -99;
  khoo2  = -99;
  khoo3  = -99;
  khoo4  = -99;
}

                                                                                                                                                      TrackInfo2.cc                                                                                       0000644 0047266 0051066 00000001272 13064405373 013035  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "TrackInfo2.h"

#include <cmath>

using std::sin;
using std::cos;
using std::sqrt;

ClassImp(TrackInfo2)

void TrackInfo2::Clear(void) {
    p = 0., costh = 0., phi = 0.;   // Momentum
    r = 0., z = 0.;         // Impact parameters
    rz_svd = -1, rphi_svd = -1;  // SVD hits
    atckpi = -2;       // Identification
    atckp  = -2;
    eid    = -2;
    muid   = -2;
}

double TrackInfo2::pz(void) const { return p * costh;}
double TrackInfo2::pt(void) const { return p * sqrt(1. - costh * costh);}
double TrackInfo2::px(void) const { return pt() * cos(phi);}
double TrackInfo2::py(void) const { return pt() * sin(phi);}

#ifdef __MAKECINT__
#pragma link C++ class TrackInfo2;
#endif

                                                                                                                                                                                                                                                                                                                                      TrackInfo.cc                                                                                        0000644 0047266 0051066 00000001124 12770400276 012747  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "TrackInfo.h"
#include <cmath>

ClassImp(TrackInfo)

void TrackInfo::Clear(void){
  p[0] = -99, p[1] = -99, p[2] = -99;   // Momentum
  r = -99, z = -99;         // Impact parameters
  rz_svd = -1, rphi_svd = -1;// SVD hits
  atckpi = -2;       // Identification
  atckp  = -2;
  eid    = -2;
  muid   = -2;
}

double TrackInfo::P(void) const{
  return sqrt(p[0]*p[0]+p[1]*p[1]+p[2]*p[2]);
}

double TrackInfo::pt(void) const{
  return sqrt(p[0]*p[0]+p[1]*p[1]);
}

double TrackInfo::costh(void) const{
  return p[2]/P();
}

#ifdef __MAKECINT__
#pragma link C++ class TrackInfo;
#endif

                                                                                                                                                                                                                                                                                                                                                                                                                                            trkinfo.cc                                                                                          0000644 0047266 0051066 00000001555 12770400276 012553  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "trkinfo.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

//////////////
//  Tracks  //
//////////////
TrkUserInfo::TrkUserInfo() :
  m_rz_svd_hits(0),
  m_rphi_svd_hits(0),
  m_atckpi(0.),
  m_atckp(0.),
  m_eid(0.),
  m_muid(0.),
  m_r(0.),
  m_z(0.)
{
}

TrkUserInfo::~TrkUserInfo()
{
}

TrkUserInfo::TrkUserInfo(const TrkUserInfo &x)
{
  *this = x;
}

TrkUserInfo* TrkUserInfo::clone(void) const
{
  TrkUserInfo *x = new TrkUserInfo( *this );
  return x;
}

TrkUserInfo & TrkUserInfo::operator = (const TrkUserInfo &x)
{
  m_rz_svd_hits   = x.m_rz_svd_hits;
  m_rphi_svd_hits = x.m_rphi_svd_hits;
  m_atckpi        = x.m_atckpi;
  m_atckp         = x.m_atckp;
  m_eid           = x.m_eid;
  m_muid          = x.m_muid;
  m_r             = x.m_r;
  m_z             = x.m_z;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
                                                                                                                                                   uisetter.cc                                                                                         0000644 0047266 0051066 00000021201 13066613146 012732  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                // Base on example from J. Tanaka

#include <cmath>
#include <vector>

#include "uisetter.h"
#include "recotools.h"

#include "particle/Particle.h"
#include "kid/atc_pid.h"
#include "mdst/mdst.h"
#include "belle.h"
#include "CLHEP/Geometry/Point3D.h"
#include <CLHEP/Vector/ThreeVector.h>
#include "helix/Helix.h"
#include "ip/IpProfile.h"
#include MDST_H
#include <mdst/Muid_mdst.h>
#include "geninfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

using std::fabs;
using std::cerr;
using std::endl;
using std::abs;
using std::vector;

bool   UISetter::m_mc_flag  = false;
int    UISetter::m_rphi_svd = 1;
int    UISetter::m_rz_svd   = 2;
double UISetter::m_trk_pt   = 0.05;
double UISetter::m_atckpi_k = 0.2;
double UISetter::m_r_ip     = 2;
double UISetter::m_z_ip     = 5;

int UISetter::CheckTrk(Particle& trk, const bool wUI) {
    if (trk.p().perp() < m_trk_pt) return 1;
    if (wUI) {
        const TrkUserInfo& tinfo = static_cast<const TrkUserInfo&>(trk.userInfo());
        if (tinfo.rphi_svd_hits() < m_rphi_svd) return 2;
        if (tinfo.rz_svd_hits() < m_rz_svd)     return 3;
        if (fabs(tinfo.r()) > m_r_ip)           return 4;
        if (fabs(tinfo.z()) > m_z_ip)           return 5;
        if (abs(trk.lund()) != 211)
            if(tinfo.atckpi() < m_atckpi_k)     return 6;
    } else {
        int rphi, rz;
        double r, z;
        GetTrkFitInfo(trk, rphi, rz, r, z);
        if (rphi    < m_rphi_svd) return 2;
        if (rz      < m_rz_svd)   return 3;
        if (fabs(r) > m_r_ip)     return 4;
        if (fabs(z) > m_z_ip)     return 5;
        if (abs(trk.lund()) != 211){
            const double atckpi = GetKPiID(trk);
            if (atckpi < m_atckpi_k) return 6;
        }   
    }
    return 0;
}

void UISetter::GetTrkFitInfo(const Particle& trk,
                             int& rphi, int& rz, double& r, double& z) {
    const int hypo = abs(trk.lund()) == 211 ? 2 : 3;//2 -> pi, 3 -> K
    Mdst_trk_fit &trk_fit = trk.mdstCharged().trk().mhyp(hypo);
    rphi = trk_fit.nhits(3);
    rz   = trk_fit.nhits(4);

    HepPoint3D pivot(trk_fit.pivot(0),trk_fit.pivot(1),trk_fit.pivot(2));
    HepVector a(5);
    for (int i = 0; i < 5; i++) a[i] = trk_fit.helix(i);

    Helix helix(pivot, a);
    helix.pivot(IpProfile::e_position());

    r = helix.dr();
    z = helix.dz();
}

double UISetter::GetKPiID(Particle& trk) {
    atc_pid selKpi(3, 1, 5, 3, 2);
    return selKpi.prob(trk.mdstCharged());
}

double UISetter::GetKPID(Particle& trk) {
    atc_pid selKpi(3, 1, 5, 3, 4);
    return selKpi.prob(trk.mdstCharged());
}

double UISetter::GetEID(Particle& trk) {
    eid sel_e(trk.mdstCharged());
    return sel_e.prob(3, -1, 5);
}

double UISetter::GetMuID(Particle& trk) {
    Muid_mdst muID(trk.mdstCharged());
    if (!muID.Status())  return -3;
    if (muID.Chi_2() <= 0) return -4;
    return muID.Muon_likelihood();
}

void UISetter::SetTrkInfo(Particle& trk) {
    int rphi,rz;
    double r,z;
    GetTrkFitInfo(trk,rphi,rz,r,z);
    TrkUserInfo& tinfo = dynamic_cast<TrkUserInfo&>(trk.userInfo());
    tinfo.rphi_svd_hits(rphi);
    tinfo.rz_svd_hits(rz);
    tinfo.r(r);
    tinfo.z(z);
    tinfo.atckpi(GetKPiID(trk));
    tinfo.atckp(GetKPID(trk));
    tinfo.eid(GetEID(trk));
    tinfo.muid(GetMuID(trk));
}

void UISetter::SetD0lInfo(vector<Particle> &d0l) {
    for (unsigned i = 0; i < d0l.size(); i++) {
        d0l[i].userInfo(D0UserInfo());
        dynamic_cast<D0UserInfo&>(d0l[i].userInfo()).Mass(d0l[i].p().m());
        if (m_mc_flag) setMCtruth(d0l[i]);
    }
}

void UISetter::SetDlInfo(vector<Particle> &dl) {
    for (unsigned i = 0; i < dl.size(); i++) {
        dl[i].userInfo(DUserInfo());
        dynamic_cast<DUserInfo&>(dl[i].userInfo()).Mass(dl[i].p().m());
        if(m_mc_flag) setMCtruth(dl[i]);
    }
}

void UISetter::SetPhilInfo(vector<Particle> &phil) {
    for (unsigned i = 0; i < phil.size(); i++) {
        phil[i].userInfo(PhiUserInfo());
        if(m_mc_flag) setMCtruth(phil[i]);
    }
}

void UISetter::SetKst0lInfo(vector<Particle> &kst0l) {
    for (unsigned i = 0; i < kst0l.size(); i++) {
        kst0l[i].userInfo(PhiUserInfo());
        if(m_mc_flag) setMCtruth(kst0l[i]);
    }
}

void UISetter::Seth0lInfo(vector<Particle> &etal) {
    for (unsigned i = 0; i < etal.size(); i++) {
        etal[i].userInfo(EtaUserInfo());
        dynamic_cast<EtaUserInfo&>(etal[i].userInfo()).Mass(etal[i].p().m());
        if (m_mc_flag) setMCtruth(etal[i]);
    }
}

void UISetter::SetPi0lInfo(vector<Particle> &pi0l) {
    for (unsigned i = 0; i < pi0l.size(); i++) {
        pi0l[i].userInfo(Pi0UserInfo());
        dynamic_cast<Pi0UserInfo&>(pi0l[i].userInfo()).Mass(pi0l[i].p().m());
        if(m_mc_flag) setMCtruth(pi0l[i]);
    }
}

void UISetter::SetModeD0(vector<Particle> &pl, const int mode) {
    for (unsigned i = 0; i < pl.size(); i++)
        dynamic_cast<D0UserInfo&>(pl[i].userInfo()).Mode(mode);
}

void UISetter::SetModeD(vector<Particle> &pl, const int mode) {
    for (unsigned i = 0; i < pl.size(); i++)
        dynamic_cast<DUserInfo&>(pl[i].userInfo()).Mode(mode);
}

void UISetter::SetModeB(vector<Particle> &pl, const int mode) {
    for (unsigned i = 0; i < pl.size(); i++)
        dynamic_cast<BUserInfo&>(pl[i].userInfo()).Mode(mode);
}

void UISetter::SetModeEta(std::vector<Particle> &pl, const int mode) {
    for (unsigned i = 0; i < pl.size(); i++)
        dynamic_cast<EtaUserInfo&>(pl[i].userInfo()).Mode(mode);
}

void UISetter::SetBadVtx(Particle& b0) {
    dynamic_cast<B0UserInfo&>(b0.userInfo()).ChisqSig(-55);
}

void UISetter::SetBadMass(Particle& b0) {
    dynamic_cast<B0UserInfo&>(b0.userInfo()).MassChi2(-55);
}

void UISetter::SetSigVtxFitInfo(Particle& b0, kvertexfitter& kvf) {
    B0UserInfo& binfo = dynamic_cast<B0UserInfo&>(b0.userInfo());
    binfo.ZSig(kvf.vertex().z());
    binfo.SzSig(kvf.errVertex()[2][2]);
    binfo.NdfSig(kvf.dgf_tracks());
    binfo.ChisqSig(kvf.chisq_tracks());
    binfo.ClSig(kvf.cl());
    binfo.NtrkSig(3);
}

void UISetter::SetB0toDhhMassFitInfo(Particle& b0, kmassfitter& kmf) {
    if (kmf.tracks() != 3) {
        cerr << "UIS::SetB0toDhhMassFitInfo: wrong tracks number: "
             << kmf.tracks() << endl;
        return;
    }
    double hel = 0;
    double mass = 0;
    const HepLorentzVector b0lv = kmf.momentum(0) + kmf.momentum(1) 
                                                  + kmf.momentum(2);
    B0UserInfo& binfo = dynamic_cast<B0UserInfo&>(b0.userInfo());

    hel = RTools::ResHelicity(b0lv,kmf.momentum(0),kmf.momentum(1),mass);
    binfo.CosHel12(hel);
    binfo.Mass12(mass);

    hel = RTools::ResHelicity(b0lv,kmf.momentum(0),kmf.momentum(2),mass);
    binfo.CosHel13(hel);
    binfo.Mass13(mass);

    hel = RTools::ResHelicity(b0lv,kmf.momentum(1),kmf.momentum(2),mass);
    binfo.CosHel23(hel);
    binfo.Mass23(mass);

    binfo.MassChi2(kmf.chisq()/kmf.dgf());
}

void UISetter::SetB0toDhhRawInfo(Particle& b0) {
    if (b0.nChildren() != 3) {
        cerr << "UIS::SetB0toDhhRawInfo: wrong children number: "
             << b0.nChildren() << endl;
        return;
    }
    double hel = 0;
    double mass = 0;
    const HepLorentzVector& b0lv = b0.p();
    const HepLorentzVector& p1 = b0.child(0).p();
    const HepLorentzVector& p2 = b0.child(1).p();
    const HepLorentzVector& p3 = b0.child(2).p();
    B0UserInfo& binfo = dynamic_cast<B0UserInfo&>(b0.userInfo());

    hel = RTools::ResHelicity(b0lv, p1, p2, mass);
    binfo.CosHel12(hel);
    binfo.Mass12(mass);

    hel = RTools::ResHelicity(b0lv, p1, p3, mass);
    binfo.CosHel13(hel);
    binfo.Mass13(mass);

    hel = RTools::ResHelicity(b0lv, p2, p3, mass);
    binfo.CosHel23(hel);
    binfo.Mass23(mass);
}

void UISetter::SetMbc_deltaE(Particle& b0,
                             const double& mbc, const double& de) {
    BUserInfo& binfo = dynamic_cast<BUserInfo&>(b0.userInfo());
    binfo.Mbc(mbc);
    binfo.deltaE(de);
}

void UISetter::SetTagVtxFitInfo(Particle& b0, TagVK& tagv) {
    B0UserInfo& binfo = dynamic_cast<B0UserInfo&>(b0.userInfo());
    binfo.ZAsc(tagv.vtx().z());
    binfo.SzAsc(tagv.errVtx()[2][2]);
    binfo.NtrkAsc((int)tagv.ntrk());
    binfo.NdfAsc((int)tagv.ndf_tracks());
    binfo.ChisqAsc(tagv.chisq_tracks());
    binfo.ClAsc(tagv.cl());
    binfo.tagl(tagv.isTagLeptonVertex());
    binfo.taglid(tagv.isTagLeptonVertex() ?
                 tagv.VertexTagLepton().get_ID() : 0);
}

void UISetter::SetRawDtoKspipiVars(Particle &d0) {
    if (d0.nChildren() != 3) {
        cerr << "D0 children: " << d0.nChildren() << "!" << endl;
        return;
    }
    const double mp = (d0.child(0).p() + d0.child(1).p()).m2();
    const double mm = (d0.child(0).p() + d0.child(2).p()).m2();
    dynamic_cast<D0UserInfo&>(d0.userInfo()).SetRawDalitzVars(mp, mm);
}

#if defined(BELLE_NAMESPACE)
}
#endif 

                                                                                                                                                                                                                                                                                                                                                                                               VertexInfo.cc                                                                                       0000644 0047266 0051066 00000000230 12770400276 013155  0                                                                                                    ustar   vitaly                          b_belle                                                                                                                                                                                                                #include "VertexInfo.h"

ClassImp(VertexInfo)

void VertexInfo::Clear(void){
  z = -99.; sz = -99.;
  ntrk = -1; ndf = -1;
  chisq = -99.; cl = -99.;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        