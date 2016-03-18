#include "kinefitter.h"

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

