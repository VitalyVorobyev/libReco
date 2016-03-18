#include "ksinfo.h"

using namespace std;

#if defined(BELLE_NAMESPACE)
namespace Belle {
#endif 

//////////////
//    Ks    //
//////////////
KsUserInfo::KsUserInfo() :
  m_dr(0),
  m_dz(0),
  m_dphi(0.),
  m_fl(0.),
  m_m_raw(0.)
{
}

KsUserInfo::~KsUserInfo()
{
}

KsUserInfo::KsUserInfo(const KsUserInfo &x)
  : m_dr(x.m_dr),
    m_dz(x.m_dz),
    m_dphi(x.m_dphi),
    m_fl(x.m_fl),
    m_m_raw(x.m_m_raw)
{
}

KsUserInfo* KsUserInfo::clone(void) const
{
  KsUserInfo *x = new KsUserInfo( *this );
  return x;
}

KsUserInfo & KsUserInfo::operator = (const KsUserInfo &x)
{
  m_dr = x.m_dr;
  m_dz = x.m_dz;
  m_dphi = x.m_dphi;
  m_fl = x.m_fl;
  m_m_raw = x.m_m_raw;
  return *this;
}


#if defined(BELLE_NAMESPACE)
}
#endif 
