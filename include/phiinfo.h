#ifndef PHIINFO_H
#define PHIINFO_H

#include "particle/ParticleUserInfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

class PhiUserInfo : public ParticleUserInfo{
public:
  PhiUserInfo();
  PhiUserInfo(const PhiUserInfo &);
  virtual ~PhiUserInfo();
  PhiUserInfo * clone(void) const;
  PhiUserInfo & operator = (const PhiUserInfo &);
public:
  void   Mass(const double& v) {m_mass = v;}
  double Mass(void) const {return m_mass;}

  void   Chi2(const double& v) {m_chi2 = v;}
  double Chi2(void) const {return m_chi2;}

  void mcflag(const int v) {m_mcflag = v;}
  int  mcflag(void) const {return m_mcflag;}

private:
  double m_mass;
  double m_chi2;
  int    m_mcflag;
};

#if defined(BELLE_NAMESPACE)
}
#endif 

#endif
