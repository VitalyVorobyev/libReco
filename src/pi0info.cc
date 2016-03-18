#include "pi0info.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///////////////
//    pi0    //
///////////////
Pi0UserInfo::Pi0UserInfo() :
  m_mass(0),
  m_chi2(0),
  m_fit_flag(-1),
  m_gamma_energy(0),
  m_mode(-1),
  m_gamma_pt(0)
{
}

Pi0UserInfo::~Pi0UserInfo()
{
}

Pi0UserInfo::Pi0UserInfo(const Pi0UserInfo &x)
  : m_mass(x.m_mass),
    m_chi2(x.m_chi2),
    m_gamma_energy(x.m_gamma_energy),
    m_gamma_pt(x.m_gamma_pt),
    m_fit_flag(x.m_fit_flag),
    m_mode(x.m_mode)
{
}

Pi0UserInfo* Pi0UserInfo::clone(void) const
{
  Pi0UserInfo *x = new Pi0UserInfo( *this );
  return x;
}

Pi0UserInfo & Pi0UserInfo::operator = (const Pi0UserInfo &x)
{
  m_mass = x.m_mass;
  m_chi2 = x.m_chi2;
  m_gamma_energy = x.m_gamma_energy;
  m_gamma_pt = x.m_gamma_pt;
  m_fit_flag = x.m_fit_flag;
  m_mode = x.m_mode;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
