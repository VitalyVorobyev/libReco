#ifndef __KINEFITTER_H__
#define __KINEFITTER_H__

#include "particle/Particle.h"
#include "kfitter/kvertexfitter.h"
#include "kfitter/kmassfitter.h"
#include "kfitter/kmassvertexfitter.h"

#include <vector>

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif

class KineFitter{
public:
  KineFitter() {}

// * General fitters with kfitter * //
  static kvertexfitter     vtx_fit(Particle& p,const bool wIPtube,int& flag);
  static kmassvertexfitter mass_vtx_fit(Particle& p);
  static kmassfitter       mass_fit(Particle& p, const bool atvtx);
// * Recursive fitters * //
  static int vtx_fit_r(Particle& p,double& chisq);
  static int mass_fit_r(Particle& p,double& chisq);
// * Ad hoc fitters * //
  static int d0tokspipi_vtx_fit(Particle& d0);
  static int d0tokspipi_dtf(Particle& d0);
  static int pi0_mass_fit(Particle& pi0);
  static int b0tod0pipi_pipivtx_fit(Particle& b0);
  static int b0tod0pipi_d0vtx_fit(Particle& b0);

private:
  static int make_mother(Particle& p, kvertexfitter& kf);
  static int make_mother(Particle& p, kmassfitter& kf);
  static void errGam(HepSymMatrix& errCart, const Mdst_gamma& gamma);
};

#if defined(BELLE_NAMESPACE)
}
#endif

#endif
