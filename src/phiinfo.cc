#include "phiinfo.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

///////////////
//    phi    //
///////////////
PhiUserInfo::PhiUserInfo() :
  m_mass(0),
  m_chi2(-1),
  m_mcflag(0)
{
}

PhiUserInfo::~PhiUserInfo()
{
}

PhiUserInfo::PhiUserInfo(const PhiUserInfo &x)
  : m_mass(x.m_mass),
    m_chi2(x.m_chi2),
    m_mcflag(x.m_mcflag)
{
}

PhiUserInfo* PhiUserInfo::clone(void) const
{
  PhiUserInfo *x = new PhiUserInfo( *this );
  return x;
}

PhiUserInfo & PhiUserInfo::operator = (const PhiUserInfo &x)
{
  m_mass   = x.m_mass;
  m_chi2   = x.m_chi2;
  m_mcflag = x.m_mcflag;
  return *this;
}

#if defined(BELLE_NAMESPACE)
}
#endif 
