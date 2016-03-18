#ifndef KSINFO_H
#define KSINFO_H

#include "particle/ParticleUserInfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

class KsUserInfo : public ParticleUserInfo{
public:
  KsUserInfo();
  KsUserInfo(const KsUserInfo &);
  virtual ~KsUserInfo();
  KsUserInfo * clone(void) const;
  KsUserInfo & operator = (const KsUserInfo &);
public:
  void dr(const double& v) {m_dr = v;}
  double dr(void) const {return m_dr;}

  void dz(const double& v) {m_dz = v;}
  double dz(void) const {return m_dz;}

  void dphi(const double& v) {m_dphi = v;}
  double dphi(void) const {return m_dphi;}

  void fl(const double& v) {m_fl = v;}
  double fl(void) const {return m_fl;}

  void Mass(const double& v) {m_m_raw = v;}
  double Mass(void) const {return m_m_raw;}
private:
  double m_dr,m_dz,m_dphi,m_fl;
  double m_m_raw;
//  bool m_mass_fit_done;
};

#if defined(BELLE_NAMESPACE)
}
#endif 

#endif
