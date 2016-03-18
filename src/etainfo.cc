#include "etainfo.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///////////////
//    eta    //
///////////////
EtaUserInfo::EtaUserInfo() :
  m_mass(0),
  m_chi2(-1),
  m_mode(-1),
  m_gamma_energy(0)
{
}

EtaUserInfo::~EtaUserInfo()
{
}

EtaUserInfo::EtaUserInfo(const EtaUserInfo &x) 
{
  *this = x;
}

EtaUserInfo* EtaUserInfo::clone(void) const
{
  EtaUserInfo *x = new EtaUserInfo(*this);
  return x;
}

EtaUserInfo & EtaUserInfo::operator = (const EtaUserInfo &x)
{
  m_mass = x.m_mass;
  m_chi2 = x.m_chi2;
  m_gamma_energy = x.m_gamma_energy;
  m_mode = x.m_mode;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
