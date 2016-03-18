#ifndef PI0INFO_H
#define PI0INFO_H

#include "particle/ParticleUserInfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

class Pi0UserInfo : public ParticleUserInfo{
public:
  Pi0UserInfo();
  Pi0UserInfo(const Pi0UserInfo &);
  virtual ~Pi0UserInfo();
  Pi0UserInfo * clone(void) const;
  Pi0UserInfo & operator = (const Pi0UserInfo &);
public:
  void Mass(const double& v) {m_mass = v;}
  double Mass(void) const {return m_mass;}

  void Chi2(const double& v) {m_chi2 = v;}
  double Chi2(void) const {return m_chi2;}

  void FitFlag(const int v) {m_fit_flag = v;}
  int FitFlag(void) const {return m_fit_flag;}

  void Mode(const int v) {m_mode = v;}
  int Mode(void) const {return m_mode;}

  void EGamma(const double& v) {m_gamma_energy = v;}
  double EGamma(void) {return m_gamma_energy;}

  void PtGamma(const double& v) {m_gamma_pt = v;}
  double PtGamma(void) {return m_gamma_pt;}

private:
  double m_mass;
  double m_chi2;
  double m_gamma_energy;
  double m_gamma_pt;
  int m_fit_flag;
  int m_mode;
};

#if defined(BELLE_NAMESPACE)
}
#endif 

#endif
