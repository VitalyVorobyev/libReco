#ifndef ETAINFO_H
#define ETAINFO_H

#include "particle/ParticleUserInfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

class EtaUserInfo : public ParticleUserInfo {
 public:
    EtaUserInfo();
    EtaUserInfo(const EtaUserInfo &);
    virtual ~EtaUserInfo();
    EtaUserInfo * clone(void) const;
    EtaUserInfo & operator = (const EtaUserInfo &);
 public:
    void Mass(const double& v) {m_mass = v;}
    double Mass(void) const {return m_mass;}

    void Chi2(const double& v) {m_chi2 = v;}
    double Chi2(void) const {return m_chi2;}

    void Mode(const int v) {m_mode = v;}
    int Mode(void) const {return m_mode;}

    void EGamma(const double& v) {m_gamma_energy = v;}
    double EGamma(void) {return m_gamma_energy;}

 private:
    int m_mode; // 1 -> gg, 2 -> pi+pi-pi0, 12 -> [g->e+e-]g, 13 -> [g->e+e-][g->e+e-]
    double m_mass;
    double m_chi2;
    double m_gamma_energy;
};

#if defined(BELLE_NAMESPACE)
}
#endif 

#endif
