#ifndef D0INFO_H
#define D0INFO_H

#include "particle/ParticleUserInfo.h"
#include "dinfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///
/// \brief Class supposed to be used for D0 -> Ks0 pi+ pi- candidates
///
class D0UserInfo : public DUserInfo{
public:
  D0UserInfo();
  D0UserInfo(const D0UserInfo &);
  virtual ~D0UserInfo();
  D0UserInfo * clone(void) const;
  D0UserInfo & operator = (const D0UserInfo &);
public:
  void SetDalitzVars(const double& m2p, const double& m2m) {m_m2p = m2p; m_m2m = m2m;}
  double m2p(void) const {return m_m2p;}
  double m2m(void) const {return m_m2m;}

  void SetRawDalitzVars(const double& m2p, const double& m2m) {m_m2p_raw = m2p; m_m2m_raw = m2m;}
  double m2p_raw(void) const {return m_m2p_raw;}
  double m2m_raw(void) const {return m_m2m_raw;}

private:
  double m_m2p,m_m2m;
  double m_m2p_raw,m_m2m_raw;
};

#if defined(BELLE_NAMESPACE)
}
#endif 

#endif
