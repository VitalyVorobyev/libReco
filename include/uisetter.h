// Used in conjunction with the particle class to store various useful numbers
// as part of particle.

#if !defined(USERINFO_H_INCLUDED)
#define USERINFO_H_INCLUDED

#include <vector>

#include "belle.h"
//#include "particle/ParticleUserInfo.h"
#include "particle/Particle.h"
#include "tagv/TagV.h"
#include "kfitter/kvertexfitter.h"
#include "kfitter/kmassfitter.h"
#include <eid/eid.h>

#include "pi0info.h"
#include "trkinfo.h"
#include "etainfo.h"
#include "phiinfo.h"
#include "ksinfo.h"
#include "d0info.h"
#include "b0info.h"
#include "dinfo.h"

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///
/// \brief Class for manipulation with UserInfo objects
///
class UISetter {
 public:
    UISetter() {}
    ~UISetter() {}
    static void SetMC(const bool mc_flag) {m_mc_flag = mc_flag;}

    static void SetD0lInfo(std::vector<Particle> &d0l);
    static void SetDlInfo(std::vector<Particle> &dl);
    static void SetPhilInfo(std::vector<Particle> &phil);
    static void SetKst0lInfo(std::vector<Particle> &kst0l);
    static void Seth0lInfo(std::vector<Particle> &etal);
    static void SetPi0lInfo(std::vector<Particle> &pi0l);
    static void  SetRawDtoKspipiVars(Particle &d0);
    static void SetMbc_deltaE(Particle& b0,const double& mbc,const double& de);

    static void SetModeD0(std::vector<Particle> &pl, const int mode);
    static void SetModeD(std::vector<Particle> &pl, const int mode);
    static void SetModeB(std::vector<Particle> &pl, const int mode);
    static void SetModeEta(std::vector<Particle> &pl, const int mode);

    static void SetBadVtx(Particle& b0);
    static void SetBadMass(Particle& b0);
    static void SetSigVtxFitInfo(Particle& b0,kvertexfitter& kvf);
    static void SetTagVtxFitInfo(Particle& b0,TagVK& tagv);
    static void SetB0toDhhMassFitInfo(Particle& b0, kmassfitter& kmf);
    static void SetB0toDhhRawInfo(Particle& b0);

    static void  SetTrkInfo(Particle& trk);
    static void GetTrkFitInfo(const Particle& trk,
                              int& rphi,int& rz,double& r,double& z);
    static int  CheckTrk(Particle& trk,const bool wUI);

    static double GetKPiID(Particle& trk);
    static double GetKPID(Particle& trk);
    static double GetEID(Particle& trk);
    static double GetMuID(Particle& trk);

    static int m_rphi_svd;
    static int m_rz_svd;
    static double m_trk_pt;
    static double m_atckpi_k;
    static double m_r_ip;
    static double m_z_ip;

 private:
    static bool m_mc_flag;
};

#if defined(BELLE_NAMESPACE)
}
#endif 

#endif
