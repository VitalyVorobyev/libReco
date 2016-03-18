#ifndef __BINFO_H_
#define __BINFO_H_

#include "particle/ParticleUserInfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///
/// \brief Simple class for B meson info. Base class for other classes
///
class BUserInfo : public ParticleUserInfo{
public:
  BUserInfo(void);
  BUserInfo(const BUserInfo &);
  virtual ~BUserInfo();
  BUserInfo * clone(void) const;
  BUserInfo & operator = (const BUserInfo &);
public:
  void deltaE(const double& v) {m_de = v;}
  double deltaE(void) const {return m_de;}

  void Mbc(const double& v) {m_mbc = v;}
  double Mbc(void) const {return m_mbc;}

  void Mode(const int v) {m_mode = v;}
  int Mode(void) const {return m_mode;}

private:
  double m_mbc;
  double m_de;
  int    m_mode;
};

#if defined(BELLE_NAMESPACE)
}
#endif 

#endif
