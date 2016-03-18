#ifndef DINFO_H
#define DINFO_H

#include "particle/ParticleUserInfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///
/// \brief Class for general information about D0, D+ and Ds candidates
///
class DUserInfo : public ParticleUserInfo{
public:
  DUserInfo();
  DUserInfo(const DUserInfo &);
  virtual ~DUserInfo();
  DUserInfo * clone(void) const;
  DUserInfo & operator = (const DUserInfo &);
public:
  void   Mass(const double& v) {m_mass = v;}
  double Mass(void) const {return m_mass;}

  void VtxFitDone(void) {m_vfit_done = true;}
  bool IsVtxFitDone(void) const {return m_vfit_done;}

  void VtxFitGood(const bool v) {m_vfit_good = v;}
  bool IsVtxFitGood(void) const {return m_vfit_good;}

  void   VtxChi2(const double& v) {m_vtx_chi2 = v;}
  double VtxChi2(void) const {return m_vtx_chi2;}

  void MassFitDone(void) {m_mfit_done = true;}
  bool IsMassFitDone(void) const {return m_mfit_done;}

  void MassFitGood(const bool v) {m_mfit_good = v;}
  bool IsMassFitGood(void) const {return m_mfit_good;}

  void   MassChi2(const double& v) {m_mass_chi2 = v;}
  double MassChi2(void) const {return m_mass_chi2;}

  void Mode(const int v) {m_mode = v;}
  int  Mode(void) const {return m_mode;}
private:
  double m_mass;

  bool   m_vfit_done;
  bool   m_vfit_good;
  double m_vtx_chi2;

  bool   m_mfit_done;
  bool   m_mfit_good;
  double m_mass_chi2;

  int m_mode;
};

#if defined(BELLE_NAMESPACE)
}
#endif 

#endif
