// Base on example from J. Tanaka

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

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

bool   UISetter::m_mc_flag  = false;
int    UISetter::m_rphi_svd = 1;
int    UISetter::m_rz_svd   = 2;
double UISetter::m_trk_pt   = 0.05;
double UISetter::m_atckpi_k = 0.2;
double UISetter::m_r_ip     = 2;
double UISetter::m_z_ip     = 5;

int UISetter::CheckTrk(Particle& trk,const bool wUI){
  if(trk.p().perp()<UISetter::m_trk_pt) return 1;
  if(wUI){
    if(((const TrkUserInfo&)trk.userInfo()).rphi_svd_hits()<UISetter::m_rphi_svd) return 2;
    if(((const TrkUserInfo&)trk.userInfo()).rz_svd_hits()<UISetter::m_rz_svd)     return 3;
    if(fabs(((const TrkUserInfo&)trk.userInfo()).r())>UISetter::m_r_ip)           return 4;
    if(fabs(((const TrkUserInfo&)trk.userInfo()).z())>UISetter::m_z_ip)           return 5;
    if(abs(trk.lund()) != 211){
      if(((const TrkUserInfo&)trk.userInfo()).atckpi()<UISetter::m_atckpi_k)      return 6;
    }
  } else{
    int rphi,rz;
    double r,z;
    UISetter::GetTrkFitInfo(trk,rphi,rz,r,z);
    if(rphi   <UISetter::m_rphi_svd) return 2;
    if(rz     <UISetter::m_rz_svd)   return 3;
    if(fabs(r)>UISetter::m_r_ip)     return 4;
    if(fabs(z)>UISetter::m_z_ip)     return 5;
    if(abs(trk.lund()) != 211){
      const double atckpi = UISetter::GetKPiID(trk);
      if(atckpi<UISetter::m_atckpi_k) return 6;
    }   
  }
  return 0;
}

void UISetter::GetTrkFitInfo(Particle& trk,int& rphi,int& rz,double& r,double& z){
  const int hypo = abs(trk.lund()) == 211 ? 2 : 3;//2 -> pi, 3 -> K
  Mdst_trk_fit &trk_fit = trk.mdstCharged().trk().mhyp(hypo);
  rphi = trk_fit.nhits(3);
  rz   = trk_fit.nhits(4);

  HepPoint3D pivot(trk_fit.pivot(0),trk_fit.pivot(1),trk_fit.pivot(2));
  HepVector a(5);
  for(int i=0;i<5;i++) a[i] = trk_fit.helix(i);

  Helix helix(pivot,a);
  helix.pivot(IpProfile::e_position());

  r = helix.dr();
  z = helix.dz();
  return; 
}

double UISetter::GetKPiID(Particle& trk){
  atc_pid selKpi(3,1,5,3,2);
  return selKpi.prob(trk.mdstCharged());
}

double UISetter::GetKPID(Particle& trk){
  atc_pid selKpi(3,1,5,3,4);
  return selKpi.prob(trk.mdstCharged());
}

double UISetter::GetEID(Particle& trk){
  eid sel_e(trk.mdstCharged());
  return sel_e.prob(3,-1,5);
}

double UISetter::GetMuID(Particle& trk){
  Muid_mdst muID(trk.mdstCharged());
  if(!muID.Status())  return -3;
  if(muID.Chi_2()<=0) return -4;
  return muID.Muon_likelihood();
}

int UISetter::SetTrkInfo(Particle& trk){
  int rphi,rz;
  double r,z;
  GetTrkFitInfo(trk,rphi,rz,r,z);
  dynamic_cast<TrkUserInfo&>(trk.userInfo()).rphi_svd_hits(rphi);
  dynamic_cast<TrkUserInfo&>(trk.userInfo()).rz_svd_hits(rz);
  dynamic_cast<TrkUserInfo&>(trk.userInfo()).r(r);
  dynamic_cast<TrkUserInfo&>(trk.userInfo()).z(z);
  dynamic_cast<TrkUserInfo&>(trk.userInfo()).atckpi(GetKPiID(trk));
  dynamic_cast<TrkUserInfo&>(trk.userInfo()).atckp(GetKPID(trk));
  dynamic_cast<TrkUserInfo&>(trk.userInfo()).eid(GetEID(trk));
  dynamic_cast<TrkUserInfo&>(trk.userInfo()).muid(GetMuID(trk));

  return 0;
}

//void UISetter::SetBlInfo(std::vector<Particle> &bl){
//  for(unsigned i=0; i<bl.size(); i++){
//    bl[i].userInfo(BUserInfo());
//    if(m_mc_flag) setMCtruth(bl[i]);
//  }
//}

void UISetter::SetD0lInfo(std::vector<Particle> &d0l){
  for(unsigned i=0; i<d0l.size(); i++){
    d0l[i].userInfo(D0UserInfo());
    dynamic_cast<D0UserInfo&>(d0l[i].userInfo()).Mass(d0l[i].p().m());
    if(m_mc_flag) setMCtruth(d0l[i]);
  }
}

void UISetter::SetDlInfo(std::vector<Particle> &dl){
  for(unsigned i=0; i<dl.size(); i++){
    dl[i].userInfo(DUserInfo());
    dynamic_cast<DUserInfo&>(dl[i].userInfo()).Mass(dl[i].p().m());
    if(m_mc_flag) setMCtruth(dl[i]);
  }
}

void UISetter::SetPhilInfo(std::vector<Particle> &phil){
  for(unsigned i=0; i<phil.size(); i++){
    phil[i].userInfo(PhiUserInfo());
    if(m_mc_flag) setMCtruth(phil[i]);
  }
}

void UISetter::SetKst0lInfo(std::vector<Particle> &kst0l){
  for(unsigned i=0; i<kst0l.size(); i++){
    kst0l[i].userInfo(PhiUserInfo());
    if(m_mc_flag) setMCtruth(kst0l[i]);
  }
}

void UISetter::Seth0lInfo(std::vector<Particle> &etal){
  for(unsigned i=0; i<etal.size(); i++){
    etal[i].userInfo(EtaUserInfo());
    dynamic_cast<EtaUserInfo&>(etal[i].userInfo()).Mass(etal[i].p().m());
    if(m_mc_flag) setMCtruth(etal[i]);
  }
}

void UISetter::SetPi0lInfo(std::vector<Particle> &pi0l){
  for(unsigned i=0; i<pi0l.size(); i++){
    pi0l[i].userInfo(Pi0UserInfo());
    dynamic_cast<Pi0UserInfo&>(pi0l[i].userInfo()).Mass(pi0l[i].p().m());
    if(m_mc_flag) setMCtruth(pi0l[i]);
  }
}

void UISetter::SetModeD0(std::vector<Particle> &pl, const int mode){
  for(unsigned i=0; i<pl.size(); i++){
    dynamic_cast<D0UserInfo&>(pl[i].userInfo()).Mode(mode);
  }
}

void UISetter::SetModeD(std::vector<Particle> &pl, const int mode){
  for(unsigned i=0; i<pl.size(); i++){
    dynamic_cast<DUserInfo&>(pl[i].userInfo()).Mode(mode);
  }
}

void UISetter::SetModeB(std::vector<Particle> &pl, const int mode){
  for(unsigned i=0; i<pl.size(); i++){
    dynamic_cast<BUserInfo&>(pl[i].userInfo()).Mode(mode);
  }
}

void UISetter::SetModeEta(std::vector<Particle> &pl, const int mode){
  for(unsigned i=0; i<pl.size(); i++){
    dynamic_cast<EtaUserInfo&>(pl[i].userInfo()).Mode(mode);
  }
}

void UISetter::SetBadVtx(Particle& b0){
  dynamic_cast<B0UserInfo&>(b0.userInfo()).ChisqSig(-55);
}

void UISetter::SetBadMass(Particle& b0){
  dynamic_cast<B0UserInfo&>(b0.userInfo()).MassChi2(-55);
}

void UISetter::SetSigVtxFitInfo(Particle& b0,kvertexfitter& kvf){
  dynamic_cast<B0UserInfo&>(b0.userInfo()).ZSig(kvf.vertex().z());
  dynamic_cast<B0UserInfo&>(b0.userInfo()).SzSig(kvf.errVertex()[2][2]);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).NdfSig(kvf.dgf_tracks());
  dynamic_cast<B0UserInfo&>(b0.userInfo()).ChisqSig(kvf.chisq_tracks());
  dynamic_cast<B0UserInfo&>(b0.userInfo()).ClSig(kvf.cl());
  dynamic_cast<B0UserInfo&>(b0.userInfo()).NtrkSig(3);
}

void UISetter::SetB0toDhhMassFitInfo(Particle& b0, kmassfitter& kmf){
  if(kmf.tracks() != 3){
    cout << "UISetter::SetB0toDhhMassFitInfo: wrong tracks number: " << kmf.tracks() << endl;
    return;
  }
  double hel = 0;
  double mass = 0;
  const HepLorentzVector b0lv = kmf.momentum(0) + kmf.momentum(1) + kmf.momentum(2);

  hel = RTools::ResonanceHelicity(b0lv,kmf.momentum(0),kmf.momentum(1),mass);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).CosHel12(hel);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).Mass12(mass);

  hel = RTools::ResonanceHelicity(b0lv,kmf.momentum(0),kmf.momentum(2),mass);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).CosHel13(hel);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).Mass13(mass);

  hel = RTools::ResonanceHelicity(b0lv,kmf.momentum(1),kmf.momentum(2),mass);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).CosHel23(hel);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).Mass23(mass);

  dynamic_cast<B0UserInfo&>(b0.userInfo()).MassChi2(kmf.chisq()/kmf.dgf());
}

void UISetter::SetB0toDhhRawInfo(Particle& b0){
  if(b0.nChildren() != 3){
    cout << "UISetter::SetB0toDhhRawInfo: wrong children number: " << b0.nChildren() << endl;
    return;
  }
  double hel = 0;
  double mass = 0;
  const HepLorentzVector b0lv = b0.p();
  const HepLorentzVector p1 = b0.child(0).p();
  const HepLorentzVector p2 = b0.child(1).p();
  const HepLorentzVector p3 = b0.child(2).p();

  hel = RTools::ResonanceHelicity(b0lv,p1,p2,mass);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).RawCosHel12(hel);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).RawMass12(mass);

  hel = RTools::ResonanceHelicity(b0lv,p1,p3,mass);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).RawCosHel13(hel);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).RawMass13(mass);

  hel = RTools::ResonanceHelicity(b0lv,p2,p3,mass);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).RawCosHel23(hel);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).RawMass23(mass);
}

void UISetter::SetMbc_deltaE(Particle& b0,const double& mbc,const double& de){
  dynamic_cast<BUserInfo&>(b0.userInfo()).Mbc(mbc);
  dynamic_cast<BUserInfo&>(b0.userInfo()).deltaE(de);
}

void UISetter::SetTagVtxFitInfo(Particle& b0,TagVK& tagv){
  dynamic_cast<B0UserInfo&>(b0.userInfo()).ZAsc(tagv.vtx().z());
  dynamic_cast<B0UserInfo&>(b0.userInfo()).SzAsc(tagv.errVtx()[2][2]);
  dynamic_cast<B0UserInfo&>(b0.userInfo()).NtrkAsc((int)tagv.ntrk());
  dynamic_cast<B0UserInfo&>(b0.userInfo()).NdfAsc((int)tagv.ndf_tracks());
  dynamic_cast<B0UserInfo&>(b0.userInfo()).ChisqAsc(tagv.chisq_tracks());
  dynamic_cast<B0UserInfo&>(b0.userInfo()).ClAsc(tagv.cl());
  dynamic_cast<B0UserInfo&>(b0.userInfo()).tagl(tagv.isTagLeptonVertex());
  dynamic_cast<B0UserInfo&>(b0.userInfo()).taglid(tagv.isTagLeptonVertex() ? tagv.VertexTagLepton().get_ID() : 0);
}

int UISetter::SetRawDtoKspipiVars(Particle &d0){
  if(d0.nChildren() != 3){
    cout << "D0 children: " << d0.nChildren() << "!" << endl;
    return -1;
  }
  const HepLorentzVector mom_p = d0.child(0).p() + d0.child(1).p();
  const HepLorentzVector mom_m = d0.child(0).p() + d0.child(2).p();
  const double mp = mom_p.m2();
  const double mm = mom_m.m2();
  dynamic_cast<D0UserInfo&>(d0.userInfo()).SetRawDalitzVars(mp,mm);
  return 0;
}

#if defined(BELLE_NAMESPACE)
}
#endif 

