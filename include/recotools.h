#ifndef __RECOTOOLS_H__
#define __RECOTOOLS_H__

#include "particle/Particle.h"
#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Matrix/Matrix.h>
#include <CLHEP/Matrix/SymMatrix.h>
#include <vector>
#include HEPEVT_H
#include "datastructures.h"
#include "ksfwmoments.h"
#include "b0info.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif

class RTools{
public:
  RTools() {}

  static double Helicity(const Particle& p,const int nch = 0);
  static double Helicity(const HepLorentzVector& mov, HepLorentzVector chv);
  static double ResonanceHelicity(const HepLorentzVector& mov, const HepLorentzVector& ch1v, const HepLorentzVector& ch2v, double& invmsq);
  static double ResonanceHelicity(const Particle& b0,const int nch1,const int nch2,double& invmsq);
  static int Mbc_deltaE(const Particle& b0,double& mbc,double& de);
  static double cosThetaCMS(const Particle& b0);
  static double MissingMass(const Particle& b0);
  static double CosThr(const Particle& b0,double& thr_sig,double& thr_oth);
  static void KspipiDalitzVars(const Particle& d0,double& mpsq,double& mmsq);

  static int make_MDLH_b0_tag(Particle &b0);
  static int make_NN_b0_tag(Particle &b0);
  static int TagVertex(Particle& b0);

  static bool IsTheSame(const Particle& p1, const Particle& p2);
  static bool IsDuplicated(const Particle& p);
  static bool MyCheckSame(const Particle &p1,const Particle &p2);

  static void MyCombination(std::vector<Particle> &new_p,const Ptype &ptype,std::vector<Particle> &p1,std::vector<Particle> &p2,const double &massL,const double &massR);
  static void MyCombination(std::vector<Particle> &new_p,const Ptype &ptype,std::vector<Particle> &p1,std::vector<Particle> &p2);
// Neutrals via Miyabayashi-san //
  static int gamma_tight(Hep3Vector gamma_3v);
  static int ChldIndex(const Particle& p,const int ch,const bool absflag);

// * MC tools * //
  static int mc_list(std::vector<Gen_hepevt>& mcl);
  static int GetTrueVertices(const Gen_hepevt& sigb, const std::vector<Gen_hepevt>& mcl, double& sigv, double& tagv, double& upsv);
  static HepLorentzVector mc_lvtx(const Gen_hepevt& b0);
  static HepLorentzVector mc_lmom(const Gen_hepevt& b0);
  static const Gen_hepevt& mc_child(const Gen_hepevt& mo, const std::vector<Gen_hepevt>& mcl, const int ch, const bool absflag);

// * Filling the standard data structured * //
  static void FillKs0(const Particle& ks,Ks0Info& info);
  static void FillPi0(const Particle& pi0, Pi0Info& info);
  static void FillKSFW(ksfwmoments& km,SFWMInfo& info,const int usefs);
  static void FillBVtx(const B0UserInfo& binfo,VertexInfo& recvtx,VertexInfo& ascvtx,const HepPoint3D& IP);
  static void FillTrk(const Particle& trk,TrackInfo& info);
  static void FillGamma(const Particle& g,GammaInfo& info);
  static void FillShape(const Particle& b,EvtShape& shape);
  static void FillEvtInfo(EvtInfo& info);
  static void FillIPBoost(IPBoost& ipb);
  static void FillHamletTag(Particle& b0,HamletTag& ht);
  static void FillGenHepEvt(GenHepEvt& ghe);
  static void FillGenPInfo(const Particle& p, GenParticleInfo& info);
private:
  static int BoostedList(const Particle& b0,std::vector<Hep3Vector>&sigtrk,std::vector<Hep3Vector>&otherb);

  static void MyCombinationCut(std::vector<Particle> &new_p,const Ptype &ptype,Particle &p1,Particle &p2,const double &massL,const double &massR);
  static void MyCombinationNoCut(std::vector<Particle> &new_p,const Ptype &ptype,Particle &p1,Particle &p2);

  static const Gen_hepevt& OtherB(const Gen_hepevt& sigb, const std::vector<Gen_hepevt>& mcl);
};

#if defined(BELLE_NAMESPACE)
}
#endif

#endif

