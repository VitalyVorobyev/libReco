#ifndef TRKINFO_H
#define TRKINFO_H

#include "particle/ParticleUserInfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

class TrkUserInfo : public ParticleUserInfo{
public:
  TrkUserInfo();
  TrkUserInfo(const TrkUserInfo &);
  virtual ~TrkUserInfo();
  TrkUserInfo * clone(void) const;
  TrkUserInfo & operator = (const TrkUserInfo &);
public:
// charged pi
  void   rz_svd_hits(const int v) {m_rz_svd_hits = v;}
  int    rz_svd_hits(void) const  {return m_rz_svd_hits;}

  void   rphi_svd_hits(const int v) {m_rphi_svd_hits = v;}
  int    rphi_svd_hits(void) const  {return m_rphi_svd_hits;}

  void   atckpi(const double& v) {m_atckpi = v;}
  double atckpi(void) const {return m_atckpi;}

  void   atckp(const double& v) {m_atckp = v;}
  double atckp(void) const {return m_atckp;}

  void   eid(const double& v) {m_eid = v;}
  double eid(void) const {return m_eid;}

  void   muid(const double& v) {m_muid = v;}
  double muid(void) const {return m_muid;}

  void   r(const double v) {m_r = v;}
  double r(void) const {return m_r;}

  void   z(const double v) {m_z = v;}
  double z(void) const {return m_z;}
private:
// charged pions
  int m_rz_svd_hits;
  int m_rphi_svd_hits;
  double m_atckpi;
  double m_atckp;
  double m_eid;
  double m_muid;
  double m_r,m_z;
};

#if defined(BELLE_NAMESPACE)
}
#endif 

#endif
