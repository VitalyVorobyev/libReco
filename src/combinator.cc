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
//#include BELLETDF_H
//#include EVTCLS_H

using namespace std;
#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif

bool   Combinator::m_ntuple_flag = false;
bool   Combinator::m_mc_flag     = false;
double Combinator::m_d0mmin      = 1.86484  - 0.065;
double Combinator::m_d0mmax      = 1.86484  + 0.065;
double Combinator::m_d0mmin_narrow = 1.86484- 0.035;
double Combinator::m_d0mmax_narrow = 1.86484+ 0.035;
double Combinator::m_dpmmin      = 1.86961  - 0.065;
double Combinator::m_dpmmax      = 1.86961  + 0.065;
double Combinator::m_mds_min     = 1.967    - 0.065;
double Combinator::m_mds_max     = 1.967    + 0.065;
double Combinator::m_mphi_min    = 1.020    - 0.020;
double Combinator::m_mphi_max    = 1.020    + 0.020;
double Combinator::m_omega_min   = 0.78265  - 0.030;
double Combinator::m_omega_max   = 0.78265  + 0.030;
double Combinator::m_eta_min_gg  = 0.547862 - 0.035;
double Combinator::m_eta_max_gg  = 0.547862 + 0.035;
double Combinator::m_eta_min_ppp = 0.547862 - 0.020;
double Combinator::m_eta_max_ppp = 0.547862 + 0.020;
double Combinator::m_egamma_min  = 0.08;
double Combinator::m_egamma_pi0_min = 0.04;
double Combinator::m_kst0mmin    = 0.89166  - 0.050;
double Combinator::m_kst0mmax    = 0.89166  + 0.050;
double Combinator::m_dsst_dm_min = 0.1438   - 0.007;
double Combinator::m_dsst_dm_max = 0.1438   + 0.007;
double Combinator::m_dst0_dm_min = 0.14212  - 0.007;
double Combinator::m_dst0_dm_max = 0.14212  + 0.007;
double Combinator::m_dstp_dm_min = 0.1454257- 0.007;
double Combinator::m_dstp_dm_max = 0.1454257+ 0.007;
double Combinator::m_mdsstj_min  = 2.15;
double Combinator::m_mdsstj_max  = 2.75;

int Combinator::make_kpi(vector<Particle> &pipl,vector<Particle> &piml,vector<Particle> &kpl,vector<Particle> &kml){
  pipl.clear(); piml.clear(); kpl.clear(); kml.clear();
  const Gen_hepevt &null = Gen_hepevt_Manager::get_manager().get_NULL();

  Mdst_charged_Manager &chg_mgr = Mdst_charged_Manager::get_manager();
  for(vector<Mdst_charged>::const_iterator i = chg_mgr.begin(); i != chg_mgr.end(); i++){
    const Mdst_charged& track = *i;
    Particle pi(track,Ptype(track.charge()>0 ? "PI+" : "PI-"));
    Particle K(track,Ptype(track.charge()>0  ? "K+"  : "K-" ));

    if(m_ntuple_flag){
      pi.userInfo(TrkUserInfo());
      K.userInfo(TrkUserInfo());
      UISetter::SetTrkInfo(pi);
      UISetter::SetTrkInfo(K);

      const int flag = UISetter::CheckTrk(K,true);
      if(flag>0 && flag!=6) continue;
      const double rip = fabs(((const TrkUserInfo&)K.userInfo()).r());
      if(!(rip<2)){
        cout << "r = " << rip << ", flag = " << flag << endl;
      }
      const Gen_hepevt &h = m_mc_flag ? get_hepevt(track) : null;
      if(h){
        pi.relation().genHepevt(h);
        K.relation().genHepevt(h);
      }
      if(track.charge() > 0){
        pipl.push_back(pi);
        if(flag != 6) kpl.push_back(K);
      } else{
        piml.push_back(pi);
        if(flag != 6) kml.push_back(K);
      }
    } else{
      const int flag = UISetter::CheckTrk(K,false);
      if(flag>0 && flag!=6) continue;
      if(track.charge() > 0){
        pipl.push_back(pi);
        if(flag != 6) kpl.push_back(K);
      } else{
        piml.push_back(pi);
        if(flag != 6) kml.push_back(K);
      }
    }
  }
  return pipl.size()+piml.size();
}

int Combinator::make_ks(vector<Particle> &ksl){
  ksl.clear();
  const Gen_hepevt &null = Gen_hepevt_Manager::get_manager().get_NULL();
  nisKsFinder ksnb;

  Mdst_vee2_Manager &vee2_mgr = Mdst_vee2_Manager::get_manager();
  const HepPoint3D IP = IpProfile::usable() ? IpProfile::e_position() : HepPoint3D(0);
  for(vector<Mdst_vee2>::iterator i = vee2_mgr.begin(); i != vee2_mgr.end(); i++){
    Mdst_vee2 &vee2 = *i;
    int kind = vee2.kind();
    if(kind != 1) continue;
    ksnb.candidates(vee2,IP);
    if(!ksnb.standard()) continue;
    Particle p(vee2);
    if(p.p().m()>0.52 || p.p().m()<0.48) continue;
    const Gen_hepevt &h = m_mc_flag ? get_hepevt(vee2) : null;
    if(h) p.relation().genHepevt(h);
    if(m_ntuple_flag){
      p.userInfo(KsUserInfo());
      dynamic_cast<KsUserInfo&>(p.userInfo()).dr(ksnb.drp()>ksnb.drn() ? ksnb.drp() : ksnb.drn());
      dynamic_cast<KsUserInfo&>(p.userInfo()).dz(ksnb.zdist());
      dynamic_cast<KsUserInfo&>(p.userInfo()).dphi(ksnb.dphi());
      dynamic_cast<KsUserInfo&>(p.userInfo()).fl(ksnb.fl());
    }
    ksl.push_back(p);
  }
  return ksl.size();
}

int Combinator::make_d0tok0shh(vector<Particle> &d0l,vector<Particle> &ksl,vector<Particle> &hpl,vector<Particle> &hml,const int mode){
  d0l.clear(); combination(d0l,Ptype("D0"),ksl,hpl,hml);
  withMassCut(d0l,m_d0mmin,m_d0mmax);
  if(m_ntuple_flag){
    UISetter::SetD0lInfo(d0l);
    UISetter::SetModeD0(d0l,mode);
  }
  return d0l.size();
}

int Combinator::make_d0tohh(vector<Particle> &d0l,vector<Particle> &h1l,vector<Particle> &h2l,const int mode,const bool narrow){
  d0l.clear();
  combination(d0l,Ptype("D0"),h1l,h2l);
  if(narrow) withMassCut(d0l,m_d0mmin_narrow,m_d0mmax_narrow);
  else       withMassCut(d0l,m_d0mmin,m_d0mmax);
  if(m_ntuple_flag){
    UISetter::SetDlInfo(d0l);
    UISetter::SetModeD(d0l,mode);
  }
  return d0l.size();
}

int Combinator::make_d0tokpi(vector<Particle> &d0l,vector<Particle> &kml,vector<Particle> &pipl,vector<Particle> &kpl,vector<Particle> &piml,const int mode){
  vector<Particle> d0bl;
  make_d0tokpi(d0l,d0bl,kml,pipl,kpl,piml,mode);
  d0l.insert(d0l.end(),d0bl.begin(),d0bl.end());
  return d0l.size();
}

int Combinator::make_d0tokpi(vector<Particle> &d0l,vector<Particle> &d0bl,vector<Particle> &kml,vector<Particle> &pipl,vector<Particle> &kpl,vector<Particle> &piml,const int mode){
  d0l.clear(); d0bl.clear();
  combination(d0l,Ptype("D0"),kml,pipl);
  withMassCut(d0l,m_d0mmin,m_d0mmax);
  combination(d0bl,Ptype("D0B"),kpl,piml);
  withMassCut(d0bl,m_d0mmin,m_d0mmax);
  if(m_ntuple_flag){
    UISetter::SetDlInfo(d0l);
    UISetter::SetDlInfo(d0bl);
    UISetter::SetModeD(d0l,mode);
    UISetter::SetModeD(d0bl,mode);
  }
  return d0l.size() + d0bl.size();
}


int Combinator::make_kstar0(vector<Particle> &kst0l,vector<Particle> &kpl,vector<Particle> &piml,vector<Particle> &kml,vector<Particle> &pipl){
  vector<Particle> kst0bl;
  make_kstar0(kst0l,kst0bl,kpl,piml,kml,pipl);
  kst0l.insert(kst0l.end(),kst0bl.begin(),kst0bl.end());
  return kst0l.size();
}

int Combinator::make_kstar0(vector<Particle> &kst0l,vector<Particle> &kst0bl,vector<Particle> &kpl,vector<Particle> &piml,vector<Particle> &kml,vector<Particle> &pipl){
  kst0l.clear(); kst0bl.clear();
//  combination(kst0l,Ptype("anti-K*0"),kml,pipl);
  combination(kst0l,Ptype(-313),kml,pipl);
  withMassCut(kst0l,m_kst0mmin,m_kst0mmax);
//  combination(kst0bl,Ptype("K*0"),kpl,piml);
  combination(kst0bl,Ptype(313),kpl,piml);
  withMassCut(kst0bl,m_kst0mmin,m_kst0mmax);
  if(m_ntuple_flag){
    UISetter::SetKst0lInfo(kst0l);
    UISetter::SetKst0lInfo(kst0bl);
  }
  return kst0l.size()+kst0bl.size();
}

int Combinator::make_d0tokpipi0(vector<Particle> &d0l,vector<Particle> &kml,vector<Particle> &pipl,vector<Particle> &kpl,vector<Particle> &piml,vector<Particle> &pi0l,const int mode,const bool narrow){
  vector<Particle> d0bl;
  make_d0tokpipi0(d0l,d0bl,kml,pipl,kpl,piml,pi0l,mode,narrow);
  d0l.insert(d0l.end(),d0bl.begin(),d0bl.end());
  return d0l.size();
}

int Combinator::make_d0tokpipi0(vector<Particle> &d0l,vector<Particle> &d0bl,vector<Particle> &kml,vector<Particle> &pipl,vector<Particle> &kpl,vector<Particle> &piml,vector<Particle> &pi0l,const int mode,const bool narrow){
  d0l.clear(); d0bl.clear();
  combination(d0l,Ptype("D0"),kml,pipl,pi0l);
  combination(d0bl,Ptype("D0B"),kpl,piml,pi0l);
  if(narrow){
    withMassCut(d0l,m_d0mmin_narrow,m_d0mmax_narrow);
    withMassCut(d0bl,m_d0mmin_narrow,m_d0mmax_narrow);
  } else{
    withMassCut(d0l,m_d0mmin,m_d0mmax);
    withMassCut(d0bl,m_d0mmin,m_d0mmax);
  }
  if(m_ntuple_flag){
    UISetter::SetD0lInfo(d0l);
    UISetter::SetD0lInfo(d0bl);
    UISetter::SetModeD(d0l,mode);
    UISetter::SetModeD(d0bl,mode);
  }
  return d0l.size()+d0bl.size();
}

int Combinator::make_d0tokpipipi(vector<Particle> &d0l,vector<Particle> &kml,vector<Particle> &pipl,vector<Particle> &kpl,vector<Particle> &piml,const int mode,const bool narrow){
  vector<Particle> d0bl;
  make_d0tokpipipi(d0l,d0bl,kml,pipl,kpl,piml,mode,narrow);
  d0l.insert(d0l.end(),d0bl.begin(),d0bl.end());
  return d0l.size();
}

int Combinator::make_d0tokpipipi(vector<Particle> &d0l,vector<Particle> &d0bl,vector<Particle> &kml,vector<Particle> &pipl,vector<Particle> &kpl,vector<Particle> &piml,const int mode,const bool narrow){
  d0l.clear(); d0bl.clear();
  combination(d0l,Ptype("D0"),pipl,piml,pipl,kml);
  combination(d0bl,Ptype("D0B"),piml,pipl,piml,kpl);
  if(narrow){
    withMassCut(d0l,m_d0mmin_narrow,m_d0mmax_narrow);
    withMassCut(d0bl,m_d0mmin_narrow,m_d0mmax_narrow);
  } else{
    withMassCut(d0l,m_d0mmin,m_d0mmax);
    withMassCut(d0bl,m_d0mmin,m_d0mmax);
  }
  if(m_ntuple_flag){
    UISetter::SetDlInfo(d0l);
    UISetter::SetDlInfo(d0bl);
    UISetter::SetModeD(d0l,mode);
    UISetter::SetModeD(d0bl,mode);
  }
  return d0l.size()+d0bl.size();
}

int Combinator::make_dptokpipi(vector<Particle> &dpl,vector<Particle> &kml,vector<Particle> &pipl,vector<Particle> &kpl,vector<Particle> &piml,const int mode){
  vector<Particle> dml;
  make_dptokpipi(dpl,dml,kml,pipl,kpl,piml,mode);
  dpl.insert(dpl.end(),dml.begin(),dml.end());
  return dpl.size();
}

int Combinator::make_dptokpipi(vector<Particle> &dpl,vector<Particle> &dml,vector<Particle> &kml,vector<Particle> &pipl,vector<Particle> &kpl,vector<Particle> &piml,const int mode){
  dpl.clear(); dml.clear();
  combination(dpl,Ptype("D+"),kml,pipl,pipl);
  withMassCut(dpl,m_dpmmin,m_dpmmax);
  combination(dml,Ptype("D-"),kpl,piml,piml);
  withMassCut(dml,m_dpmmin,m_dpmmax);
  if(m_ntuple_flag){
    UISetter::SetDlInfo(dpl);
    UISetter::SetDlInfo(dml);
    UISetter::SetModeD(dpl,mode);
    UISetter::SetModeD(dml,mode);
  }
  return dpl.size() + dml.size();
}

int Combinator::make_pi0(vector<Particle> &pi0l){
  pi0l.clear();
  const Gen_hepevt &null = Gen_hepevt_Manager::get_manager().get_NULL();
// pi0 > gamma gamma
  Mdst_pi0_Manager &pi0_mgr = Mdst_pi0_Manager::get_manager();
  for(vector<Mdst_pi0>::const_iterator i = pi0_mgr.begin(); i != pi0_mgr.end(); i++){
    const Mdst_pi0& pi0 = *i;
    Particle p(pi0);
    if(p.child(0).p().t() < m_egamma_pi0_min || p.child(1).p().t() < m_egamma_pi0_min) continue;
    if(m_ntuple_flag){
      const Gen_hepevt &h = m_mc_flag ? get_hepevt(pi0) : null;
      if(h) p.relation().genHepevt(h);
      const Gen_hepevt &hg1 = m_mc_flag ? get_hepevt(p.child(0).mdstGamma()) : null;
      if(hg1) p.child(0).relation().genHepevt(hg1);
      const Gen_hepevt &hg2 = m_mc_flag ? get_hepevt(p.child(1).mdstGamma()) : null;
      if(hg2) p.child(1).relation().genHepevt(hg2);
      p.userInfo(Pi0UserInfo());
      dynamic_cast<Pi0UserInfo&>(p.userInfo()).Mass(pi0.mass());
      dynamic_cast<Pi0UserInfo&>(p.userInfo()).Chi2(pi0.chisq());
      setGammasError(p,HepPoint3D(0),1.*HepSymMatrix(3,1));// Necessary for kinematic fitting
    }
    pi0l.push_back(p);
  }
  return pi0l.size();
}

void Combinator::setH0toGGErrors(vector<Particle>& etal){
//  const HepPoint3D origin;
//  HepSymMatrix dx = HepSymMatrix(3,1)*1.0;
//  setGammaError(eta.child(0),origin,dx);
//  setGammaError(eta.child(1),origin,dx);
  for(int i=0; i<etal.size(); i++) setGammasError(etal[i],HepPoint3D(0),1.*HepSymMatrix(3,1));
}

int Combinator::make_gamma(vector<Particle>& gl){
  gl.clear(); makeGamma(gl); withPCut(gl,m_egamma_min);
  return gl.size();
}

int Combinator::make_etato2g(vector<Particle>& etal, vector<Particle>& gammas){
  etal.clear();
// eta -> gamma gamma
  gammas.clear();
  makeGamma(gammas); withPCut(gammas,0.08);
  combination(etal,Ptype(221),gammas,gammas);
  withMassCut(etal,m_eta_min_gg,m_eta_max_gg);
  if(m_ntuple_flag){
    UISetter::Seth0lInfo(etal);
    Combinator::setH0toGGErrors(etal);
  }
  return etal.size();
}

int Combinator::make_etato3pi(vector<Particle>& etal,vector<Particle>& pipl,vector<Particle>& piml,vector<Particle>& pi0l){
  etal.clear();
  combination(etal,Ptype(221),pipl,piml,pi0l);
  withMassCut(etal,m_eta_min_ppp,m_eta_max_ppp);
  if(m_ntuple_flag) UISetter::Seth0lInfo(etal);
  return etal.size();
}

int Combinator::make_omega(vector<Particle>& omegal,vector<Particle>& pipl,vector<Particle>& piml,vector<Particle>& pi0l){
  omegal.clear();
// omega -> pi+ pi- pi0
  combination(omegal,Ptype(223),pipl,piml,pi0l);
  withMassCut(omegal,m_omega_min,m_omega_max);
  if(m_ntuple_flag) UISetter::Seth0lInfo(omegal);
  return omegal.size();
}

int Combinator::make_phitokk(vector<Particle> &phil,vector<Particle> &kpl,vector<Particle> &kml){
  phil.clear();
  combination(phil,Ptype("PHI"),kpl,kml);
  withMassCut(phil,m_mphi_min,m_mphi_max);
  if(m_ntuple_flag) UISetter::SetPhilInfo(phil);
  return phil.size();
}

int Combinator::make_dstoh0hp(vector<Particle> &dsl,vector<Particle> &h0l,vector<Particle> &hpl,vector<Particle> &hml,const int mode){
  vector<Particle> dsml;
  make_dstoh0hp(dsl,dsml,h0l,hpl,hml,mode);
  dsl.insert(dsl.end(),dsml.begin(),dsml.end());
  return dsl.size();
}

int Combinator::make_dstoh0hp(vector<Particle> &dspl,vector<Particle> &dsml,vector<Particle> &h0l,vector<Particle> &hpl,vector<Particle> &hml,const int mode){
  dspl.clear(); dsml.clear();
//  combination(dspl,Ptype("D_s+"),h0l,hpl);
//  combination(dsml,Ptype("D_s-"),h0l,hml);
  combination(dspl,Ptype( 431),h0l,hpl);
  combination(dsml,Ptype(-431),h0l,hml);
  withMassCut(dspl,m_mds_min,m_mds_max);
  withMassCut(dsml,m_mds_min,m_mds_max);
  if(m_ntuple_flag){
    UISetter::SetDlInfo(dspl);
    UISetter::SetDlInfo(dsml);
    UISetter::SetModeD(dspl,mode);
    UISetter::SetModeD(dsml,mode);
  }
  return dspl.size()+dsml.size();
}

int Combinator::make_dstoKK(vector<Particle> &dsl,vector<Particle> &kst0bl,vector<Particle> &kpl,vector<Particle> &kst0l,vector<Particle> &kml,const int mode){
  vector<Particle> dsml;
  make_dstoKK(dsl,dsml,kst0bl,kpl,kst0l,kml,mode);
  dsl.insert(dsl.end(),dsml.begin(),dsml.end());
  return dsl.size();
}

int Combinator::make_dstoKK(vector<Particle> &dspl,vector<Particle> &dsml,vector<Particle> &kst0bl,vector<Particle> &kpl, vector<Particle> &kst0l,vector<Particle> &kml,const int mode){
  dspl.clear(); dsml.clear();
//  combination(dspl,Ptype("D_s+"),kst0bl,kml);
//  combination(dsml,Ptype("D_s-"),kst0l,kpl);
  combination(dspl,Ptype(431),kst0bl,kml);
  combination(dsml,Ptype(-431),kst0l,kpl);
  withMassCut(dspl,m_mds_min,m_mds_max);
  withMassCut(dsml,m_mds_min,m_mds_max);
  if(m_ntuple_flag){
    UISetter::SetDlInfo(dspl);
    UISetter::SetDlInfo(dsml);
    UISetter::SetModeD(dspl,mode);
    UISetter::SetModeD(dsml,mode);
  }
  return dspl.size()+dsml.size();
}

int Combinator::make_dsstar(vector<Particle> &dsstl,vector<Particle> &dsl,vector<Particle> &gammal){
  dsstl.clear();
  if(!dsl.size() || !gammal.size()) return 0;
//  combination(dsstl,dsl[0].pType().charge() > 0 ? Ptype("D_s*+") : Ptype("D_s*-"),dsl,gammal);
  combination(dsstl,dsl[0].pType().charge() > 0 ? Ptype(433) : Ptype(-433),dsl,gammal);
  withMassDifCut(dsstl,m_dsst_dm_min,m_dsst_dm_max,0);
  return dsstl.size();
}

int Combinator::make_dstar(vector<Particle> &dstl,vector<Particle> &dl,vector<Particle> &xl){
  dstl.clear();
  if(!dl.size() || !xl.size()) return 0;
  Ptype dstT = Ptype(413);
  double dmmin = m_dstp_dm_min;
  double dmmax = m_dstp_dm_max;
//  if(dl[0].pType() == Ptype("D_s+") && xl[0].pType() == Ptype("gamma")){
  if(dl[0].pType() == Ptype(431) && xl[0].pType() == Ptype("gamma")){
//    dstT = Ptype("D_s*+");
    dstT = Ptype(433);
    dmmin = m_dsst_dm_min; dmmax = m_dsst_dm_max;
//  } else if(dl[0].pType() == Ptype("D_s-") && xl[0].pType() == Ptype("gamma")){
  } else if(dl[0].pType() == Ptype(-431) && xl[0].pType() == Ptype("gamma")){
//    dstT = Ptype("D_s*-");
    dstT = Ptype(-433);
    dmmin = m_dsst_dm_min; dmmax = m_dsst_dm_max;
//  } else if(dl[0].pType() == Ptype("D0") && xl[0].pType() == Ptype("pi0")){
  } else if(dl[0].pType() == Ptype("D0") && xl[0].pType() == Ptype(111)){
//    dstT = Ptype("D*0");
    dstT = Ptype(423);
    dmmin = m_dst0_dm_min; dmmax = m_dst0_dm_max;
//  } else if(dl[0].pType() == Ptype("anti-D0") && xl[0].pType() == Ptype("pi0")){
  } else if(dl[0].pType() == Ptype(-421) && xl[0].pType() == Ptype(111)){
//    dstT = Ptype("anti-D*0");
    dstT = Ptype(-423);
    dmmin = m_dst0_dm_min; dmmax = m_dst0_dm_max;
//  } else if((dl[0].pType() == Ptype("D0") || dl[0].pType() == Ptype("anti-D0")) && xl[0].pType() == Ptype("pi+")){
  } else if((dl[0].pType() == Ptype("D0") || dl[0].pType() == Ptype(-421)) && xl[0].pType() == Ptype(211)){
//    dstT = Ptype("D*+");
    dstT = Ptype(413);
    dmmin = m_dstp_dm_min; dmmax = m_dstp_dm_max;
//  } else if((dl[0].pType() == Ptype("D0") || dl[0].pType() == Ptype("anti-D0")) && xl[0].pType() == Ptype("pi-")){
  } else if((dl[0].pType() == Ptype("D0") || dl[0].pType() == Ptype(-421)) && xl[0].pType() == Ptype(-211)){
//    dstT = Ptype("D*-");
    dstT = Ptype(-413);
    dmmin = m_dstp_dm_min; dmmax = m_dstp_dm_max;
  } else {
    cout << "Combinator::make_dstar: wrong particles " << dl[0].pType().lund() << " " << xl[0].pType().lund() << endl;
    return 0;
  }
  combination(dstl,dstT,dl,xl);
  withMassDifCut(dstl,dmmin,dmmax,0);
  return dstl.size();
}

int Combinator::make_dsjtodsx(vector<Particle> &dsjl,vector<Particle> &dspl,vector<Particle> &dsml,vector<Particle> &xl,const int mode){
  vector<Particle> dsjml;
  make_dsjtodsx(dsjl,dsjml,dspl,dsml,xl,mode);
  dsjl.insert(dsjl.end(),dsjml.begin(),dsjml.end());
  return dsjl.size();
}

int Combinator::make_dsjtodsx(vector<Particle> &dsjpl,vector<Particle> &dsjml,vector<Particle> &dspl,vector<Particle> &dsml,vector<Particle> &xl,const int mode){
  dsjpl.clear(); dsjml.clear();
//  cout << "make_dsjtodsx: " << dspl.size() << " " << dsml.size() << " " << xl.size() << endl;
  if(!xl.size()) return 0;
//  combination(dsjpl,Ptype("D_s+"),dspl,xl);
//  combination(dsjml,Ptype("D_s-"),dsml,xl);
  if(dspl.size()){
//    cout << "dspl1 " << dspl[0].pType().lund() << " " << xl[0].pType().lund() << endl;
    combination(dsjpl,Ptype(431),dspl,xl);
    withMassCut(dsjpl,m_mdsstj_min,m_mdsstj_max);
//    cout << "dspl2" << endl;
  }
  if(dsml.size()){
//    cout << "dsml1"  << dsml[0].pType().lund() << " " << xl[0].pType().lund() << endl;
    combination(dsjml,Ptype(-431),dsml,xl);
    withMassCut(dsjml,m_mdsstj_min,m_mdsstj_max);
//    cout << "dsml2" << endl;
  }
  if(m_ntuple_flag){
    UISetter::SetDlInfo(dsjpl);
    UISetter::SetDlInfo(dsjml);
    UISetter::SetModeD(dsjpl,mode);
    UISetter::SetModeD(dsjml,mode);
  }
//  cout << "Done" << endl;
  return dsjpl.size() + dsjml.size();
}

int Combinator::make_dsjtodsxy(vector<Particle> &dsjl,vector<Particle> &dspl,vector<Particle> &dsml,vector<Particle> &xl,vector<Particle> &yl,const int mode){
  vector<Particle> dsjml;
  make_dsjtodsxy(dsjl,dsjml,dspl,dsml,xl,yl,mode);
  dsjl.insert(dsjl.end(),dsjml.begin(),dsjml.end());
  return dsjl.size();
}

int Combinator::make_dsjtodsxy(vector<Particle> &dsjpl,vector<Particle> &dsjml,vector<Particle> &dspl,vector<Particle> &dsml,vector<Particle> &xl,vector<Particle> &yl,const int mode){
  dsjpl.clear(); dsjml.clear();
  if(!xl.size() || !yl.size()) return 0;
//  combination(dsjpl,Ptype("D_s+"),dspl,xl,yl);
//  combination(dsjml,Ptype("D_s-"),dsml,xl,yl);
  if(dspl.size()){
    combination(dsjpl,Ptype( 431),dspl,xl,yl);
    withMassCut(dsjpl,m_mdsstj_min,m_mdsstj_max);
  }
  if(dsml.size()){
    combination(dsjml,Ptype(-431),dsml,xl,yl);
    withMassCut(dsjml,m_mdsstj_min,m_mdsstj_max);
  }
  if(m_ntuple_flag){
    UISetter::SetDlInfo(dsjpl);
    UISetter::SetDlInfo(dsjml);
    UISetter::SetModeD(dsjpl,mode);
    UISetter::SetModeD(dsjml,mode);
  }
//  cout << "Done" << endl;
  return dsjpl.size() + dsjml.size();
}

int Combinator::make_b0toxy(vector<Particle>& b0l,vector<Particle>& xpl,vector<Particle>& yml,vector<Particle>& xml,vector<Particle>& ypl,const int mode){
  b0l.clear();
  const bool b0flag = xpl.size() && yml.size();
  const bool b0bflag = xml.size() && ypl.size();
//  cout << "make_b0toxy: " << xpl.size() << " " << yml.size() << " " << xml.size() << " " << ypl.size() << endl;
  if(!b0flag && !b0bflag) return 0;
  if(b0flag)  combination(b0l, Ptype("B0"),xpl,yml);
  if(b0bflag){
    vector<Particle> b0bl;
    combination(b0bl,Ptype(-511),xml,ypl);
    b0l.insert(b0l.end(),b0bl.begin(),b0bl.end());
  }
  double mbc,de;
  for(unsigned i=0; i<b0l.size(); i++){
    Particle& B0 = b0l[i];
    RTools::Mbc_deltaE(B0,mbc,de);
    if(!m_ntuple_flag){
      if(fabs(de)<0.3 && mbc>5.22 && mbc<5.30){
        if(!RTools::IsDuplicated(B0)) return 1;
      }
    } else{
      if(de>0.3 || de<-0.3 || mbc<5.22 || mbc>5.30){ b0l.erase(b0l.begin()+i); i--; continue; }
      if(RTools::IsDuplicated(B0)){                  b0l.erase(b0l.begin()+i); i--; continue; }
      B0.userInfo(BUserInfo());
      UISetter::SetMbc_deltaE(B0,mbc,de);
    }
  }
  if(m_ntuple_flag){
    UISetter::SetModeB(b0l,mode);
    return b0l.size();
  }
  return 0;
}

int Combinator::make_bptoxy(vector<Particle>& bpl,vector<Particle>& x0l,vector<Particle>& ypl,const int mode){
  bpl.clear();
  if(!x0l.size() || !ypl.size()) return 0;
  combination(bpl,ypl[0].pType().charge() > 0 ? Ptype("B+") : Ptype("B-"),x0l,ypl);
  double mbc,de;
  for(unsigned i=0; i<bpl.size(); i++){
    Particle& Bp = bpl[i];
    RTools::Mbc_deltaE(Bp,mbc,de);
    if(!m_ntuple_flag){
      if(fabs(de)<0.3 && mbc>5.22 && mbc<5.30){
//        cout << "make_bptoxy: " << i << endl;
        if(!RTools::IsDuplicated(Bp)) return 1;
      }
    } else{
      if(de>0.3 || de<-0.3 || mbc<5.22 || mbc>5.30){ bpl.erase(bpl.begin()+i); i--; continue; }
      if(RTools::IsDuplicated(Bp)){                  bpl.erase(bpl.begin()+i); i--; continue; }
      Bp.userInfo(BUserInfo());
      UISetter::SetMbc_deltaE(Bp,mbc,de);
    }
  }
  if(m_ntuple_flag){
    UISetter::SetModeB(bpl,mode);
    return bpl.size();
  }
  return 0;
}

int Combinator::make_b0tod0pipi(std::vector<Particle>& b0l,std::vector<Particle> &d0l,std::vector<Particle> &pipl, std::vector<Particle> &piml){
  b0l.clear();
  combination(b0l,Ptype("B0"),d0l,pipl,piml);
  double mbc,de;
  for(unsigned i=0; i<b0l.size(); i++){
    Particle& B0 = b0l[i];
    RTools::Mbc_deltaE(B0,mbc,de);
    if(!m_ntuple_flag){
      if(fabs(de)<0.35 && mbc>5.18 && mbc<5.31){
        if(!RTools::IsDuplicated(B0)) return 1;
      }
    } else{
      B0.userInfo(B0UserInfo());
      const Particle& D0 = B0.child(0);
//      if(B0.)
      if(de>0.3 || de<-0.3 || mbc<5.25 || mbc>5.30){ b0l.erase(b0l.begin()+i); i--; continue; }
      if(RTools::IsDuplicated(B0)){                  b0l.erase(b0l.begin()+i); i--; continue; }
      UISetter::SetB0toDhhRawInfo(B0);

      if(make_vtx_for_b0tod0pipi(B0)){
        UISetter::SetBadVtx(B0);             // b0l.erase(b0l.begin()+i); i--; continue; }
        UISetter::SetMbc_deltaE(B0,mbc,de);
        continue;
      }
      if(make_mass_for_b0tod0pipi(B0)){
        UISetter::SetBadMass(B0);             // b0l.erase(b0l.begin()+i); i--; continue; }
      }      
      UISetter::SetMbc_deltaE(B0,mbc,de);
    }
  }
  return m_ntuple_flag ? b0l.size() : 0;
}

int Combinator::make_vtx_for_b0tod0pipi(Particle& b0){
  const int d0index = RTools::ChldIndex(b0,421,true);
  if(d0index<0) return -4;// There is no D0 among children
  Particle& D0 = b0.child(d0index);
  // * Vertex fitting * //
  const bool d0_vtx_fit_done = ((const DUserInfo&)D0.userInfo()).IsVtxFitDone();
  if(!d0_vtx_fit_done){
    double d0_vtx_chisq = -1;
    const int d0_vtx_fit_flag = KineFitter::vtx_fit_r(D0,d0_vtx_chisq);
    dynamic_cast<DUserInfo&>(D0.userInfo()).VtxChi2(d0_vtx_chisq);
    dynamic_cast<DUserInfo&>(D0.userInfo()).VtxFitDone();
    dynamic_cast<DUserInfo&>(D0.userInfo()).VtxFitGood(!d0_vtx_fit_flag);
  }
  const bool d0_vtx_fit_good = ((const DUserInfo&)D0.userInfo()).IsVtxFitGood();
  if(!d0_vtx_fit_good){
    cout << "Bad D0 vtx fit for mode " << ((const DUserInfo&)D0.userInfo()).Mode() << endl;
    return -1;
  }
  int fit_flag = 0; 
  kvertexfitter kvf = KineFitter::vtx_fit(b0,true,fit_flag);
  if(fit_flag) return -2;
  UISetter::SetSigVtxFitInfo(b0,kvf);
  if(RTools::TagVertex(b0)) return -3;
  return 0;
}

int Combinator::make_mass_for_b0tod0pipi(Particle& b0){
  const int d0index = RTools::ChldIndex(b0,421,true);
  if(d0index<0) return -4;// There is no D0 among children
  Particle& D0 = b0.child(d0index);
  const bool d0_mass_fit_done = ((const DUserInfo&)D0.userInfo()).IsMassFitDone();
  if(!d0_mass_fit_done){
    double d0_mass_chisq = -1;
    const int d0_mass_fit_flag = KineFitter::mass_fit_r(D0,d0_mass_chisq);
    dynamic_cast<DUserInfo&>(D0.userInfo()).MassChi2(d0_mass_chisq);
    dynamic_cast<DUserInfo&>(D0.userInfo()).MassFitDone();
    dynamic_cast<DUserInfo&>(D0.userInfo()).MassFitGood(!d0_mass_fit_flag);
  }
  const bool d0_mass_fit_good = ((const DUserInfo&)D0.userInfo()).IsMassFitGood();
  if(!d0_mass_fit_good){
    cout << "Bad D0 mass fit for mode " << ((const DUserInfo&)D0.userInfo()).Mode() << endl;
    return -1;
  }
  kmassfitter kmf = KineFitter::mass_fit(b0,true);
  UISetter::SetB0toDhhMassFitInfo(b0,kmf);
  return 0;
}

int Combinator::make_b0tod0pi0pi0(std::vector<Particle>& b0l,std::vector<Particle> &d0l,std::vector<Particle> &pi0l){
  b0l.clear();
  combination(b0l,Ptype("B0"),d0l,pi0l,pi0l);
  double mbc,de;
  for(int i=0; i<b0l.size(); i++){
    Particle& B0 = b0l[i];
    RTools::Mbc_deltaE(B0,mbc,de);
    if(!m_ntuple_flag){
      if(fabs(de)<0.32 && mbc>5.18 && mbc<5.31){
        if(!RTools::IsDuplicated(B0)) return 1;
      }
    } else{
      B0.userInfo(B0UserInfo());
      if(de>0.3 || de<-0.3 || mbc<5.2 || mbc>5.3){
        b0l.erase(b0l.begin()+i); i--;
        continue;
      }
      if(RTools::IsDuplicated(B0)){
        b0l.erase(b0l.begin()+i); i--;
        continue;
      }
      int d0index = 0;
           if(abs(B0.child(0).lund()) == 421) d0index = 0;
      else if(abs(B0.child(1).lund()) == 421) d0index = 1;
      else if(abs(B0.child(2).lund()) == 421) d0index = 2;
      else continue;
      Particle& D0 = B0.child(d0index);
      const bool d0_vtx_fit_done = ((const DUserInfo&)D0.userInfo()).IsVtxFitDone();
      if(!d0_vtx_fit_done){
        double d0_vtx_chisq = -1;
        const int d0_vtx_fit_flag = KineFitter::vtx_fit_r(D0,d0_vtx_chisq);
        dynamic_cast<DUserInfo&>(D0.userInfo()).VtxChi2(d0_vtx_chisq);
        dynamic_cast<DUserInfo&>(D0.userInfo()).VtxFitDone();
        dynamic_cast<DUserInfo&>(D0.userInfo()).VtxFitGood(!d0_vtx_fit_flag);
      }
      const bool d0_vtx_fit_good = ((const DUserInfo&)D0.userInfo()).IsVtxFitGood();
      if(!d0_vtx_fit_good){
        b0l.erase(b0l.begin()+i); i--;
        continue;
      }
      if(KineFitter::b0tod0pipi_d0vtx_fit(B0)){
        b0l.erase(b0l.begin()+i); i--;
        continue;
      }
      if(RTools::TagVertex(B0)){
        b0l.erase(b0l.begin()+i); i--;
        continue;
      }
      UISetter::SetMbc_deltaE(B0,mbc,de);
    }
  }
  return m_ntuple_flag ? b0l.size() : 0;
}

#if defined(BELLE_NAMESPACE)
}
#endif

